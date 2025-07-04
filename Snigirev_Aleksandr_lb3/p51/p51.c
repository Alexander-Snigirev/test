/*Обеспечьте разделение памяти и необходимую синхронизацию между независимыми процессами так, чтобы один из процессов
мог записывать информацию, а другой считывать, по мере готовности и без потери данных для обоих процессов.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SHM_NAME "/shm_buffer"
#define SEM_WRITE "/sem_write"
#define SEM_READ "/sem_read"
#define BUFFER_SIZE 10
#define MESSAGE_SIZE 256
#define NUM_MESSAGES 20

// Структура кольцевого буфера в разделяемой памяти
typedef struct {
    char buffer[BUFFER_SIZE][MESSAGE_SIZE]; // Массив строк
    int read_idx;  // Индекс чтения
    int write_idx; // Индекс записи
} SharedBuffer;

void writer_process(int shm_fd, sem_t* sem_write, sem_t* sem_read) {
    // Подключаем разделяемую память
    SharedBuffer* shm = mmap(NULL, sizeof(SharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED) {
        perror("mmap failed in writer");
        exit(EXIT_FAILURE);
    }

    // Пишем сообщения
    for (int i = 0; i < NUM_MESSAGES; i++) {
        // Ждём, пока есть место для записи
        if (sem_wait(sem_write) == -1) {
            perror("sem_wait sem_write failed");
            exit(EXIT_FAILURE);
        }

        // Записываем сообщение
        snprintf(shm->buffer[shm->write_idx], MESSAGE_SIZE, "Message %d from writer", i + 1);
        printf("Writer: Wrote '%s' at index %d\n", shm->buffer[shm->write_idx], shm->write_idx);
        fflush(stdout);

        // Обновляем индекс записи
        shm->write_idx = (shm->write_idx + 1) % BUFFER_SIZE;

        // Сигнализируем читателю
        if (sem_post(sem_read) == -1) {
            perror("sem_post sem_read failed");
            exit(EXIT_FAILURE);
        }

        // Имитация работы
        usleep(200000); // 0.2 секунды
    }

    // Отключаем разделяемую память
    munmap(shm, sizeof(SharedBuffer));
}

void reader_process(int shm_fd, sem_t* sem_write, sem_t* sem_read) {
    // Подключаем разделяемую память
    SharedBuffer* shm = mmap(NULL, sizeof(SharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED) {
        perror("mmap failed in reader");
        exit(EXIT_FAILURE);
    }

    // Читаем сообщения
    for (int i = 0; i < NUM_MESSAGES; i++) {
        // Ждём, пока есть данные для чтения
        if (sem_wait(sem_read) == -1) {
            perror("sem_wait sem_read failed");
            exit(EXIT_FAILURE);
        }

        // Читаем сообщение
        printf("Reader: Read '%s' from index %d\n", shm->buffer[shm->read_idx], shm->read_idx);
        fflush(stdout);

        // Обновляем индекс чтения
        shm->read_idx = (shm->read_idx + 1) % BUFFER_SIZE;

        // Сигнализируем писателю
        if (sem_post(sem_write) == -1) {
            perror("sem_post sem_write failed");
            exit(EXIT_FAILURE);
        }

        // Имитация работы
        usleep(300000); // 0.3 секунды, чуть медленнее писателя
    }

    // Отключаем разделяемую память
    munmap(shm, sizeof(SharedBuffer));
}

int main() {
    // Создаём разделяемую память
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }

    // Устанавливаем размер разделяемой памяти
    if (ftruncate(shm_fd, sizeof(SharedBuffer)) == -1) {
        perror("ftruncate failed");
        exit(EXIT_FAILURE);
    }

    // Инициализируем разделяемую память
    SharedBuffer* shm = mmap(NULL, sizeof(SharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED) {
        perror("mmap failed in main");
        exit(EXIT_FAILURE);
    }
    shm->read_idx = 0;
    shm->write_idx = 0;
    munmap(shm, sizeof(SharedBuffer));

    // Создаём семафоры
    sem_t* sem_write = sem_open(SEM_WRITE, O_CREAT, 0666, BUFFER_SIZE); // Место для записи
    sem_t* sem_read = sem_open(SEM_READ, O_CREAT, 0666, 0); // Данные для чтения
    if (sem_write == SEM_FAILED || sem_read == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }


    // Создаём процессы
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Дочерний процесс (читатель)
        reader_process(shm_fd, sem_write, sem_read);
        exit(EXIT_SUCCESS);
    } else {
        // Родительский процесс (писатель)
        writer_process(shm_fd, sem_write, sem_read);

        // Ждём завершения читателя
        wait(NULL);

        // Очищаем ресурсы
        shm_unlink(SHM_NAME);
        sem_close(sem_write);
        sem_close(sem_read);
        sem_unlink(SEM_WRITE);
        sem_unlink(SEM_READ);
        close(shm_fd);
    }

    printf("Main process exiting.\n");
    return 0;
}