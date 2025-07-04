/*6.1. Каналы pipe. Приведите примеры использования в командной строке.
Приведите накладываемые ограничения на этот вид IPC,
включая фрагменты системных (конфигурационных) файлов.*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MESSAGE_SIZE 256
#define NUM_MESSAGES 20

void writer(int write_fd) {
    char message[MESSAGE_SIZE];

    for (int i = 0; i < NUM_MESSAGES; i++) {
        snprintf(message, MESSAGE_SIZE, "Message %d from writer", i + 1);
        printf("Writer: Sending '%s'\n", message);
        fflush(stdout);

        // Пишем в канал
        ssize_t bytes_written = write(write_fd, message, strlen(message) + 1);
        if (bytes_written == -1) {
            perror("write failed");
            exit(EXIT_FAILURE);
        }

        // Имитация работы
        usleep(300000); // 0.2 секунды
    }

    // Закрываем канал после записи
    if (close(write_fd) == -1) {
        perror("close write_fd failed");
        exit(EXIT_FAILURE);
    }
}

void reader(int read_fd) {
    char buffer[MESSAGE_SIZE];
    int messages_read = 0;

    while (1) {
        // Читаем из канала
        ssize_t bytes_read = read(read_fd, buffer, MESSAGE_SIZE);
        if (bytes_read == -1) {
            perror("read failed");
            exit(EXIT_FAILURE);
        }

        if (bytes_read == 0) {
            // Конец канала
            printf("Reader: End of pipe reached, read %d messages\n", messages_read);
            break;
        }

        printf("Reader: Received '%s'\n", buffer);
        fflush(stdout);
        messages_read++;


        // Имитация работы
        usleep(300000); // 0.3 секунды
    }

    // Закрываем канал
    if (close(read_fd) == -1) {
        perror("close read_fd failed");
        exit(EXIT_FAILURE);
    }
}

int main() {
    int pipe_fd[2]; // pipe_fd[0] - чтение, pipe_fd[1] - запись

    // Создаём канал
    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
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
        close(pipe_fd[1]); // Закрываем конец для записи
        reader(pipe_fd[0]);
        exit(EXIT_SUCCESS);
    } else {
        // Родительский процесс (писатель)
        close(pipe_fd[0]); // Закрываем конец для чтения
        writer(pipe_fd[1]);

        // Ждём завершения читателя
        wait(NULL);
    }

    printf("Main process exiting.\n");
    return 0;
}