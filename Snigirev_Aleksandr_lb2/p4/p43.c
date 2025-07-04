#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <semaphore.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define STACK_SIZE (1024 * 1024)
#define LOOP_ITERATIONS 1000000 // Уменьшил для скорости

// Глобальная переменная для проверки разделения памяти
int shared_counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Файл для записи
const char* filename = "thread_output.txt";

// Обработчик сигнала
void signal_handler(int sig) {
    printf("Signal %d received by TID=%d\n", sig, (int)gettid());
}

pid_t gettid() {
    return syscall(SYS_gettid);
}

typedef struct {
    int thread_num;
    sem_t* sem; // Для clone
} ThreadArg;

// Функция для pthread
void* pthread_func(void* arg) {
    ThreadArg* data = (ThreadArg*)arg;
    pid_t tid = gettid();
    int thread_num = data->thread_num;

    // 1. Проверка памяти
    pthread_mutex_lock(&mutex);
    shared_counter++;
    printf("pthread TID=%d, thread %d: incremented shared_counter to %d\n", 
           tid, thread_num, shared_counter);
    pthread_mutex_unlock(&mutex);

    // 2. Проверка файловых дескрипторов
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("open failed");
        return NULL;
    }
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "pthread TID=%d, thread %d wrote this\n", tid, thread_num);
    write(fd, buffer, strlen(buffer));
    close(fd);

    // 3. Проверка текущей директории
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    printf("pthread TID=%d, thread %d: current directory = %s\n", tid, thread_num, cwd);

    // 4. Имитация работы
    for (int i = 0; i < LOOP_ITERATIONS; i++);

    return NULL;
}

// Функция для clone
int clone_func(void* arg) {
    ThreadArg* data = (ThreadArg*)arg;
    pid_t tid = gettid();
    int thread_num = data->thread_num;

    // 1. Проверка памяти
    pthread_mutex_lock(&mutex);
    shared_counter++;
    printf("clone TID=%d, thread %d: incremented shared_counter to %d\n", 
           tid, thread_num, shared_counter);
    pthread_mutex_unlock(&mutex);

    // 2. Проверка файловых дескрипторов
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("open failed");
        return -1;
    }
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "clone TID=%d, thread %d wrote this\n", tid, thread_num);
    write(fd, buffer, strlen(buffer));
    close(fd);

    // 3. Проверка текущей директории
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    printf("clone TID=%d, thread %d: current directory = %s\n", tid, thread_num, cwd);

    // 4. Имитация работы
    for (int i = 0; i < LOOP_ITERATIONS; i++);

    sem_post(data->sem);
    return 0;
}

int main() {
    const int NUM_THREADS = 3;
    pid_t main_pid = getpid();
    printf("Main process PID=%d\n", main_pid);

    // Установка обработчика сигнала
    signal(SIGUSR1, signal_handler);

    // Удаляем файл, если он существует
    unlink(filename);

    // 1. Pthreads
    pthread_t pthread_ids[NUM_THREADS];
    ThreadArg pthread_args[NUM_THREADS];

    printf("Creating pthreads...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_args[i].thread_num = i + 1;
        pthread_args[i].sem = NULL;
        if (pthread_create(&pthread_ids[i], NULL, pthread_func, &pthread_args[i]) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(pthread_ids[i], NULL);
    }
    printf("All pthreads completed.\n\n");

    // 2. Clone
    pid_t clone_ids[NUM_THREADS];
    void* clone_stacks[NUM_THREADS];
    ThreadArg clone_args[NUM_THREADS];
    sem_t clone_sem;

    if (sem_init(&clone_sem, 0, 0) == -1) {
        perror("sem_init failed");
        exit(EXIT_FAILURE);
    }
    printf("Creating clone threads...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        clone_stacks[i] = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
        if (clone_stacks[i] == MAP_FAILED) {
            perror("mmap failed");
            exit(EXIT_FAILURE);
        }

        clone_args[i].thread_num = i + 1;
        clone_args[i].sem = &clone_sem;
        clone_ids[i] = clone(clone_func, clone_stacks[i] + STACK_SIZE,
                             CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD,
                             &clone_args[i]);
        if (clone_ids[i] == -1) {
            perror("clone failed");
            exit(EXIT_FAILURE);
        }
    }

    // Отправляем сигнал процессу
    kill(main_pid, SIGUSR1);

    for (int i = 0; i < NUM_THREADS; i++) {
        sem_wait(&clone_sem);
    }

    sem_destroy(&clone_sem);
    for (int i = 0; i < NUM_THREADS; i++) {
        munmap(clone_stacks[i], STACK_SIZE);
    }
    printf("All clone threads completed.\n");

    // Читаем содержимое файла
    printf("\nContents of %s:\n", filename);
    FILE* file = fopen(filename, "r");
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);

    return 0;
}