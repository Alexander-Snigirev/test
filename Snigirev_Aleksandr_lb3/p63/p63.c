/*Oсуществите передачу информации посредством очередей сообщений.
Можно ли на основе этого средства организовать «почтовый ящик»,
т.е. не синхронизируя отправителя и получателя (без ожидания доставки).*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <errno.h>

#define MQ_NAME "/my_mq"
#define MESSAGE_SIZE 256
#define NUM_MESSAGES 20
#define MQ_MAXMSG 10
#define MQ_MSGSIZE MESSAGE_SIZE

void sender(mqd_t mq) {
    char message[MESSAGE_SIZE];

    for (int i = 0; i < NUM_MESSAGES; i++) {
        // Формируем сообщение фиксированной длины
        memset(message, 0, MESSAGE_SIZE);
        snprintf(message, MESSAGE_SIZE - 1, "Message %d from sender", i + 1);
        printf("Sender: Sending '%s'\n", message);
        fflush(stdout);

        // Отправляем сообщение с приоритетом 0
        if (mq_send(mq, message, MESSAGE_SIZE, 0) == -1) {
            perror("mq_send failed");
            exit(EXIT_FAILURE);
        }

        // Имитация работы
        usleep(200000); // 0.2 секунды
    }

    // Закрываем очередь
    if (mq_close(mq) == -1) {
        perror("mq_close failed in sender");
        exit(EXIT_FAILURE);
    }
}

void receiver(mqd_t mq) {
    char buffer[MESSAGE_SIZE];
    int messages_read = 0;

    while (messages_read < NUM_MESSAGES) {
        // Получаем сообщение
        ssize_t bytes_read = mq_receive(mq, buffer, MESSAGE_SIZE, NULL);
        if (bytes_read == -1) {
            perror("mq_receive failed");
            exit(EXIT_FAILURE);
        }
        if (bytes_read != MESSAGE_SIZE) {
            fprintf(stderr, "mq_receive: incomplete read (%zd bytes)\n", bytes_read);
            exit(EXIT_FAILURE);
        }

        printf("Receiver: Received '%s'\n", buffer);
        fflush(stdout);
        messages_read++;

        // Имитация работы
        usleep(300000); // 0.3 секунды
    }

    printf("Receiver: End of queue reached, read %d messages\n", messages_read);

    // Закрываем очередь
    if (mq_close(mq) == -1) {
        perror("mq_close failed in receiver");
        exit(EXIT_FAILURE);
    }
}

int main() {
    // Настраиваем атрибуты очереди
    struct mq_attr attr;
    attr.mq_flags = 0;           // Блокирующий режим
    attr.mq_maxmsg = MQ_MAXMSG;  // Максимум 10 сообщений
    attr.mq_msgsize = MQ_MSGSIZE; // Размер сообщения 256 байт
    attr.mq_curmsgs = 0;         // Текущее количество сообщений

    // Создаём очередь
    mqd_t mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open failed");
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
        receiver(mq);
        exit(EXIT_SUCCESS);
    } else {
        // Родительский процесс (отправитель)
        sender(mq);

        // Ждём завершения получателя
        wait(NULL);

        // Удаляем очередь
        if (mq_unlink(MQ_NAME) == -1) {
            perror("mq_unlink failed");
            exit(EXIT_FAILURE);
        }
    }

    printf("Main process exiting.\n");
    return 0;
}