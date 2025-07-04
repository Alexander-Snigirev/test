//Изменится ли ситуация, если процессы заменить на потоки одного процесса. Приведите эксперимент.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MSG_COUNT 5

typedef struct {
    char message[256];
    int is_ready;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} SharedData;

void *writer(void *arg) {
    SharedData *shared = (SharedData *)arg;
    for (int i = 0; i < MSG_COUNT; i++) {
        pthread_mutex_lock(&shared->mutex);
        while (shared->is_ready) {
            pthread_cond_wait(&shared->cond, &shared->mutex);
        }
        snprintf(shared->message, sizeof(shared->message), "Message %d", i + 1);
        shared->is_ready = 1;
        printf("Writer: wrote message: %s\n", shared->message);
        pthread_cond_signal(&shared->cond);
        pthread_mutex_unlock(&shared->mutex);
        usleep(200000); // Имитация задержки записи
    }
    return NULL;
}

void *reader(void *arg) {
    SharedData *shared = (SharedData *)arg;
    for (int i = 0; i < MSG_COUNT; i++) {
        pthread_mutex_lock(&shared->mutex);
        while (!shared->is_ready) {
            pthread_cond_wait(&shared->cond, &shared->mutex);
        }
        printf("Reader: read message: %s\n", shared->message);
        shared->is_ready = 0;
        pthread_cond_signal(&shared->cond);
        pthread_mutex_unlock(&shared->mutex);
        usleep(100000); // Имитация задержки чтения
    }
    return NULL;
}

int main() {
    // Создаём структуру для данных
    SharedData shared;
    shared.is_ready = 0;

    // Инициализируем мьютекс и условную переменную
    if (pthread_mutex_init(&shared.mutex, NULL) != 0) {
        perror("pthread_mutex_init failed");
        exit(1);
    }
    if (pthread_cond_init(&shared.cond, NULL) != 0) {
        perror("pthread_cond_init failed");
        pthread_mutex_destroy(&shared.mutex);
        exit(1);
    }

    // Создаём потоки
    pthread_t writer_thread, reader_thread;
    if (pthread_create(&writer_thread, NULL, writer, &shared) != 0 ||
        pthread_create(&reader_thread, NULL, reader, &shared) != 0) {
        perror("pthread_create failed");
        pthread_mutex_destroy(&shared.mutex);
        pthread_cond_destroy(&shared.cond);
        exit(1);
    }

    // Ждём завершения потоков
    pthread_join(writer_thread, NULL);
    pthread_join(reader_thread, NULL);

    // Очищаем ресурсы
    pthread_mutex_destroy(&shared.mutex);
    pthread_cond_destroy(&shared.cond);

    printf("Main: processes completed\n");
    return 0;
}