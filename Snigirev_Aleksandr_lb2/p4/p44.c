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
#include <sys/types.h>
#include <sys/syscall.h>

#define STACK_SIZE (1024 * 1024)
#define LOOP_ITERATIONS 100000000

typedef struct {
    int thread_num;
    sem_t* sem;
} ThreadArg;

pid_t gettid() {
    return syscall(SYS_gettid);
}

// Обработчик сигнала
void signal_handler(int sig) {
    pid_t tid = gettid();
    printf("Signal %d (SIGTERM) received by TID=%d\n", sig, tid);
    fflush(stdout);
    pthread_exit(NULL); // Завершаем только текущую нить
}

// Функция для pthread
void* pthread_func(void* arg) {
    ThreadArg* data = (ThreadArg*)arg;
    pid_t tid = gettid();
    int thread_num = data->thread_num;

    printf("pthread TID=%d, thread %d started\n", tid, thread_num);
    fflush(stdout);

    for (int i = 0; i < 5; i++) {
        printf("pthread TID=%d, thread %d: iteration %d\n", tid, thread_num, i);
        fflush(stdout);
        for (int j = 0; j < LOOP_ITERATIONS; j++);
        sleep(2);
    }

    printf("pthread TID=%d, thread %d completed\n", tid, thread_num);
    return NULL;
}

// Функция для clone
int clone_func(void* arg) {
    ThreadArg* data = (ThreadArg*)arg;
    pid_t tid = gettid();
    int thread_num = data->thread_num;

    printf("clone TID=%d, thread %d started\n", tid, thread_num);
    fflush(stdout);

    for (int i = 0; i < 5; i++) {
        printf("clone TID=%d, thread %d: iteration %d\n", tid, thread_num, i);
        fflush(stdout);
        for (int j = 0; j < LOOP_ITERATIONS; j++);
        sleep(2);
    }

    printf("clone TID=%d, thread %d completed\n", tid, thread_num);
    sem_post(data->sem);
    return 0;
}

int main() {
    const int NUM_THREADS = 3;
    pid_t main_pid = getpid();
    printf("Main process PID=%d, Main TID=%d\n", main_pid, gettid());

    // Устанавливаем обработчик SIGTERM
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_RESTART; // Перезапуск системных вызовов
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }

    // НЕ блокируем SIGTERM в главной нити, чтобы все нити могли его получать

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

    printf("Main process exiting normally.\n");
    return 0;
}