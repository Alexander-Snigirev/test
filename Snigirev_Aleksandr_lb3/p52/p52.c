//Синхронизация через flock

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

#define SHM_NAME "/shm_task_5_2"
#define SHM_SIZE 1024
#define MSG_COUNT 5
#define LOCK_FILE "/tmp/task_5_2.lock"

typedef struct {
    char message[256];
    int is_ready;
} SharedData;

int main() {
    time_t start = time(NULL);
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

    // Создаём файл для блокировки
    int lock_fd = open(LOCK_FILE, O_CREAT | O_RDWR, 0644);
    if (lock_fd == -1) {
        perror("open lock file failed");
        munmap(shared, SHM_SIZE);
        shm_unlink(SHM_NAME);
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        close(lock_fd);
        unlink(LOCK_FILE);
        munmap(shared, SHM_SIZE);
        shm_unlink(SHM_NAME);
        exit(1);
    }

    if (pid == 0) {
        // Дочерний процесс (читатель)
        for (int i = 0; i < MSG_COUNT; i++) {
            while (1) {
                if (flock(lock_fd, LOCK_EX) == -1) {
                    perror("flock reader failed");
                    break;
                }
                if (shared->is_ready) {
                    printf("Reader: read message: %s\n", shared->message);
                    shared->is_ready = 0;
                    if (flock(lock_fd, LOCK_UN) == -1) {
                        perror("flock unlock reader failed");
                    }
                    break;
                }
                if (flock(lock_fd, LOCK_UN) == -1) {
                    perror("flock unlock reader failed");
                }
                usleep(10000);
            }
            usleep(100000);
        }
        close(lock_fd);
        munmap(shared, SHM_SIZE);
        exit(0);
    } else {
        // Родительский процесс (писатель)
        for (int i = 0; i < MSG_COUNT; i++) {
            if (flock(lock_fd, LOCK_EX) == -1) {
                perror("flock writer failed");
                break;
            }
            snprintf(shared->message, sizeof(shared->message), "Message %d", i + 1);
            shared->is_ready = 1;
            printf("Writer: wrote message: %s\n", shared->message);
            if (flock(lock_fd, LOCK_UN) == -1) {
                perror("flock unlock writer failed");
            }
            usleep(200000);
        }
        wait(NULL);
        close(lock_fd);
        unlink(LOCK_FILE);
        munmap(shared, SHM_SIZE);
        shm_unlink(SHM_NAME);
    }
    time_t finish = time(NULL);

    printf("Main: Processes completed\n");
    return 0;
}