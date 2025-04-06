#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <semaphore.h>
#include <time.h>
#include <sys/types.h>
#include <sys/syscall.h>

// Размер стека и итерации
#define STACK_SIZE (1024 * 1024) // 1 MB
#define LOOP_ITERATIONS 10000000000 // Длительный цикл

// Структура для передачи данных
typedef struct {
    int thread_num;
    sem_t* sem; // Для clone
} ThreadArg;

// Получение TID через системный вызов
pid_t gettid() {
    return syscall(SYS_gettid);
}

// Функция для pthread
void* pthread_func(void* arg) {
    ThreadArg* data = (ThreadArg*)arg;
    int thread_num = data->thread_num;
    pid_t tid = gettid();
    pid_t pid = getpid();
    long long sum = 0;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (long long i = 0; i < LOOP_ITERATIONS; i++) {
        sum += i % 10;
        if (i % (LOOP_ITERATIONS / 5) == 0) {
            int cpu = sched_getcpu();
            printf("pthread TID=%d, PID=%d, CPU=%d, thread %d: iteration %lld, sum = %lld\n", 
                   tid, pid, cpu, thread_num, i, sum);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("pthread TID=%d completed, final sum = %lld, time = %.3f sec\n", 
           tid, sum, time_spent);
    return NULL;
}

// Функция для clone
int clone_func(void* arg) {
    ThreadArg* data = (ThreadArg*)arg;
    int thread_num = data->thread_num;
    pid_t tid = gettid();
    pid_t pid = getpid();
    long long sum = 0;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (long long i = 0; i < LOOP_ITERATIONS; i++) {
        sum += i % 10;
        if (i % (LOOP_ITERATIONS / 5) == 0) {
            int cpu = sched_getcpu();
            printf("clone TID=%d, PID=%d, CPU=%d, thread %d: iteration %lld, sum = %lld\n", 
                   tid, pid, cpu, thread_num, i, sum);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("clone TID=%d completed, final sum = %lld, time = %.3f sec\n", 
           tid, sum, time_spent);
    sem_post(data->sem);
    return 0;
}

int main() {
    const int NUM_THREADS = 3;
    pid_t main_pid = getpid();
    printf("Main process PID=%d\n", main_pid);

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
    for (int i = 0; i < NUM_THREADS; i++) {
        sem_wait(&clone_sem);
    }

    sem_destroy(&clone_sem);
    for (int i = 0; i < NUM_THREADS; i++) {
        munmap(clone_stacks[i], STACK_SIZE);
    }
    printf("All clone threads completed.\n");

    return 0;
}