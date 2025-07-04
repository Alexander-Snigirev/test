/*Каналы fifo. Реализуйте: из командной строки и создав программу с соответствующими системными функциями.
Приведите характеристику порождаемого FIFO-файла и его содержимого,
используя: утилиты ОС, /proc и программно.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define FIFO_PATH "/tmp/myfifo"
#define MESSAGE_SIZE 256
#define NUM_MESSAGES 20

void writer(int fd) {
    char message[MESSAGE_SIZE];

    for (int i = 0; i < NUM_MESSAGES; i++) {
        // Формируем сообщение фиксированной длины
        memset(message, 0, MESSAGE_SIZE);
        snprintf(message, MESSAGE_SIZE - 1, "Message %d from writer", i + 1);
        printf("Writer: Sending '%s'\n", message);
        fflush(stdout);

        // Пишем ровно MESSAGE_SIZE байт
        ssize_t bytes_written = write(fd, message, MESSAGE_SIZE);
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

    // Закрываем дескриптор
    if (close(fd) == -1) {
        perror("close write_fd failed");
        exit(EXIT_FAILURE);
    }
}

void reader(int fd) {
    char buffer[MESSAGE_SIZE];
    int messages_read = 0;

    while (1) {
        // Читаем ровно MESSAGE_SIZE байт
        ssize_t bytes_read = read(fd, buffer, MESSAGE_SIZE);
        if (bytes_read == -1) {
            perror("read failed");
            exit(EXIT_FAILURE);
        }
        if (bytes_read == 0) {
            // Конец канала
            printf("Reader: End of FIFO reached, read %d messages\n", messages_read);
            break;
        }
        if (bytes_read != MESSAGE_SIZE) {
            fprintf(stderr, "read: incomplete read (%zd bytes)\n", bytes_read);
            exit(EXIT_FAILURE);
        }

        printf("Reader: Received '%s'\n", buffer);
        fflush(stdout);
        messages_read++;

        // Имитация работы
        usleep(300000); // 0.3 секунды
    }

    // Закрываем дескриптор
    if (close(fd) == -1) {
        perror("close read_fd failed");
        exit(EXIT_FAILURE);
    }
}

int main() {
    // Создаём FIFO
    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo failed");
        exit(EXIT_FAILURE);
    }

    // Создаём процесс
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Дочерний процесс (читатель)
        int read_fd = open(FIFO_PATH, O_RDONLY);
        if (read_fd == -1) {
            perror("open FIFO for reading failed");
            exit(EXIT_FAILURE);
        }

        reader(read_fd);
        exit(EXIT_SUCCESS);
    } else {
        // Родительский процесс (писатель)
        int write_fd = open(FIFO_PATH, O_WRONLY);
        if (write_fd == -1) {
            perror("open FIFO for writing failed");
            exit(EXIT_FAILURE);
        }

        writer(write_fd);

        // Ждём завершения читателя
        wait(NULL);

        // Удаляем FIFO
        if (unlink(FIFO_PATH) == -1) {
            perror("unlink FIFO failed");
            exit(EXIT_FAILURE);
        }
    }

    printf("Main process exiting.\n");
    return 0;
}