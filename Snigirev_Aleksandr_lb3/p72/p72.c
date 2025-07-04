#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <errno.h>

#define PORT 8080
#define MESSAGE_SIZE 256
#define NUM_MESSAGES 20

void receiver(int sock_fd) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[MESSAGE_SIZE];
    int messages_read = 0;

    while (messages_read < NUM_MESSAGES) {
        // Получаем датаграмму
        ssize_t bytes_read = recvfrom(sock_fd, buffer, MESSAGE_SIZE, 0,
                                      (struct sockaddr*)&client_addr, &client_len);
        if (bytes_read == -1) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }
        if (bytes_read != MESSAGE_SIZE) {
            fprintf(stderr, "recvfrom: incomplete read (%zd bytes)\n", bytes_read);
            continue; // Пропускаем некорректные датаграммы
        }

        printf("Receiver: Received '%s'\n", buffer);
        fflush(stdout);
        messages_read++;

        // Имитация работы
        usleep(300000); // 0.3 секунды
    }

    printf("Receiver: End of reception, read %d messages\n", messages_read);

    // Закрываем сокет
    if (close(sock_fd) == -1) {
        perror("close sock_fd failed");
        exit(EXIT_FAILURE);
    }
}

void sender(int sock_fd) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    char message[MESSAGE_SIZE];

    for (int i = 0; i < NUM_MESSAGES; i++) {
        // Формируем сообщение фиксированной длины
        memset(message, 0, MESSAGE_SIZE);
        snprintf(message, MESSAGE_SIZE - 1, "Message %d from sender", i + 1);
        printf("Sender: Sending '%s'\n", message);
        fflush(stdout);

        // Отправляем датаграмму
        ssize_t bytes_written = sendto(sock_fd, message, MESSAGE_SIZE, 0,
                                       (struct sockaddr*)&server_addr, sizeof(server_addr));
        if (bytes_written == -1) {
            perror("sendto failed");
            exit(EXIT_FAILURE);
        }
        if (bytes_written != MESSAGE_SIZE) {
            fprintf(stderr, "sendto: incomplete write (%zd bytes)\n", bytes_written);
            exit(EXIT_FAILURE);
        }

        // Имитация работы
        usleep(200000); // 0.2 секунды
    }

    // Закрываем сокет
    if (close(sock_fd) == -1) {
        perror("close sock_fd failed");
        exit(EXIT_FAILURE);
    }
}

int main() {
    // Создаём сокет для получателя
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Настраиваем адрес получателя
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Привязываем сокет (для получателя)
    if (bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Создаём процесс
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Дочерний процесс (получатель)
        receiver(sock_fd);
        exit(EXIT_SUCCESS);
    } else {
        // Родительский процесс (отправитель)
        // Даём получателю время на запуск
        usleep(100000); // 0.1 секунды

        // Создаём отдельный сокет для отправителя
        int sender_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sender_fd == -1) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        sender(sender_fd);

        // Ждём завершения получателя
        wait(NULL);

        // Закрываем сокет получателя
        if (close(sock_fd) == -1) {
            perror("close sock_fd failed");
            exit(EXIT_FAILURE);
        }
    }
    printf("Main process exiting.\n");
    return 0;
}