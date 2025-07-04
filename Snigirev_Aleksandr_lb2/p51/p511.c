#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/resource.h>

#define STACK_SIZE (1024 * 1024)
#define ITERATIONS 100 // Много квантов

typedef struct {
    char id; // Условный идентификатор (A, B, C и т.д.)
} ThreadArg;

pid_t gettid() {
    return syscall(SYS_gettid);
}

void* thread_func(void* arg) {
    ThreadArg* data = (ThreadArg*)arg;
    pid_t tid = gettid();
    printf("Thread TID=%d (%c) started\n", tid, data->id);
    fflush(stdout);

    for (int i = 0; i < ITERATIONS; i++) {
        putchar(data->id); // Вывод без перевода строки
        fflush(stdout);
        for (int j = 0; j < 1000000; j++); // Имитация работы
    }
    printf("\nThread TID=%d (%c) completed\n", tid, data->id);
    return NULL;
}

int main() {
    const int NUM_THREADS = 3;
    pid_t pid = getpid();
    printf("Main process PID=%d, TID=%d\n", pid, gettid());

    // Политика и приоритет
    int policy = sched_getscheduler(0);
    struct sched_param param;
    sched_getparam(0, &param);
    printf("Policy: %d (0=SCHED_OTHER), Priority: %d, Nice: %d\n", 
           policy, 19-getpriority(PRIO_PROCESS, 0), getpriority(PRIO_PROCESS, 0));

    // Создание нитей
    pthread_t thread_ids[NUM_THREADS];
    ThreadArg thread_args[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].id = 'A' + i; // A, B, C
        if (pthread_create(&thread_ids[i], NULL, thread_func, &thread_args[i]) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    // Главная нить тоже участвует
    for (int i = 0; i < ITERATIONS; i++) {
        putchar('M'); // M — главная нить
        fflush(stdout);
        for (int j = 0; j < 1000000; j++); // Имитация работы
    }
    printf("\nMain thread completed\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    printf("All threads completed\n");
    return 0;
}