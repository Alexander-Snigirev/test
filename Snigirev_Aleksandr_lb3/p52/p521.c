//Синхронизация через мьютексы и условные переменные

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define SHM_NAME "/shm_task_5_2_1"
#define SHM_SIZE 1024
#define MSG_COUNT 5

typedef struct {
    char message[256];
    int is_ready;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} SharedData;

int main() {
    // Создаём разделяемую память
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0644);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(1);
    }
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate failed");
        shm_unlink(SHM_NAME);
        exit(1);
    }
    SharedData *shared = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared == MAP_FAILED) {
        perror("mmap failed");
        shm_unlink(SHM_NAME);
        exit(1);
    }
    close(shm_fd);

    // Инициализируем мьютекс и условную переменную
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shared->mutex, &mattr);
    pthread_mutexattr_destroy(&mattr);

    pthread_condattr_t cattr;
    pthread_condattr_init(&cattr);
    pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&shared->cond, &cattr);
    pthread_condattr_destroy(&cattr);

    shared->is_ready = 0;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        pthread_mutex_destroy(&shared->mutex);
        pthread_cond_destroy(&shared->cond);
        munmap(shared, SHM_SIZE);
        shm_unlink(SHM_NAME);
        exit(1);
    }

    if (pid == 0) {
        // Дочерний процесс (читатель)
        for (int i = 0; i < MSG_COUNT; i++) {
            pthread_mutex_lock(&shared->mutex);
            while (!shared->is_ready) {
                pthread_cond_wait(&shared->cond, &shared->mutex);
            }
            printf("Reader: read message: %s\n", shared->message);
            shared->is_ready = 0;
            pthread_cond_signal(&shared->cond);
            pthread_mutex_unlock(&shared->mutex);
            usleep(100000);
        }
        pthread_mutex_destroy(&shared->mutex);
        pthread_cond_destroy(&shared->cond);
        munmap(shared, SHM_SIZE);
        exit(0);
    } else {
        // Родительский процесс (писатель)
        for (int i = 0; i < MSG_COUNT; i++) {
            pthread_mutex_lock(&shared->mutex);
            while (shared->is_ready) {
                pthread_cond_wait(&shared->cond, &shared->mutex);
            }
            snprintf(shared->message, sizeof(shared->message), "Message %d", i + 1);
            shared->is_ready = 1;
            printf("Writer: Wrote message: %s\n", shared->message);
            pthread_cond_signal(&shared->cond);
            pthread_mutex_unlock(&shared->mutex);
            usleep(200000);
        }
        wait(NULL);
        pthread_mutex_destroy(&shared->mutex);
        pthread_cond_destroy(&shared->cond);
        munmap(shared, SHM_SIZE);
        shm_unlink(SHM_NAME);
    }

    printf("Main: Processes completed\n");
    return 0;
}