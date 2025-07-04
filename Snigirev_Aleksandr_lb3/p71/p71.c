/*7.1. Выполните передачу информации локально посредством сокетов по TCP/IP*/
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
#define BACKLOG 5

void server(int server_fd) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Принимаем соединение
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    // Закрываем серверный сокет, так как он больше не нужен
    close(server_fd);

    char buffer[MESSAGE_SIZE];
    int messages_read = 0;

    while (messages_read < NUM_MESSAGES) {
        // Читаем сообщение
        ssize_t bytes_read = read(client_fd, buffer, MESSAGE_SIZE);
        if (bytes_read == -1) {
            perror("read failed");
            exit(EXIT_FAILURE);
        }
        if (bytes_read == 0) {
            printf("Server: Client closed connection early, read %d messages\n", messages_read);
            break;
        }
        if (bytes_read != MESSAGE_SIZE) {
            fprintf(stderr, "read: incomplete read (%zd bytes)\n", bytes_read);
            exit(EXIT_FAILURE);
        }

        printf("Server: Received '%s'\n", buffer);
        fflush(stdout);
        messages_read++;

        // Имитация работы
        usleep(300000); // 0.3 секунды
    }

    printf("Server: End of connection, read %d messages\n", messages_read);

    // Закрываем клиентский сокет
    if (close(client_fd) == -1) {
        perror("close client_fd failed");
        exit(EXIT_FAILURE);
    }
}

void client(int server_fd) {
    // Закрываем серверный сокет, так как клиент его не использует
    close(server_fd);

    // Создаём клиентский сокет
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Настраиваем адрес сервера
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // Подключаемся к серверу
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    char message[MESSAGE_SIZE];

    for (int i = 0; i < NUM_MESSAGES; i++) {
        // Формируем сообщение фиксированной длины
        memset(message, 0, MESSAGE_SIZE);
        snprintf(message, MESSAGE_SIZE - 1, "Message %d from client", i + 1);
        printf("Client: Sending '%s'\n", message);
        fflush(stdout);

        // Отправляем сообщение
        ssize_t bytes_written = write(client_fd, message, MESSAGE_SIZE);
        if (bytes_written == -1) {
            perror("write failed");
            exit(EXIT_FAILURE);
        }
        if (bytes_written != MESSAGE_SIZE) {
            fprintf(stderr, "write: incomplete write (%zd bytes)\n", bytes_written);
            exit(EXIT_FAILURE);
        }

        // Имитация работы
        usleep(200000); // 0.2 секунды
    }

    // Закрываем клиентский сокет
    if (close(client_fd) == -1) {
        perror("close client_fd failed");
        exit(EXIT_FAILURE);
    }
}

int main() {
    // Создаём серверный сокет
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Настраиваем адрес сервера
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Привязываем сокет
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Слушаем входящие соединения
    if (listen(server_fd, BACKLOG) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    // Создаём процесс
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Дочерний процесс (сервер)
        server(server_fd);
        exit(EXIT_SUCCESS);
    } else {
        // Родительский процесс (клиент)
        // Даём серверу время на запуск
        usleep(100000); // 0.1 секунды
        client(server_fd);

        // Ждём завершения сервера
        wait(NULL);
    }

    printf("Main process exiting.\n");
    return 0;
}