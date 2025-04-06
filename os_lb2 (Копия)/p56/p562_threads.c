#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <errno.h>

#define NUM_THREADS 3

void* thread_func(void* arg) {
    int thread_id = *(int*)arg;
    int policy;
    struct sched_param param;
    
    pthread_getschedparam(pthread_self(), &policy, &param);
    printf("Поток %d (TID: %ld): политика %d, приоритет %d\n", 
           thread_id, pthread_self(), policy, param.sched_priority);
    
    volatile long long i;
    for (i = 0; i < 700000000LL; i++) {
        if (i % 100000000 == 0) {
            printf("Поток %d работает (итерация %lld)\n", thread_id, i);
            fflush(stdout);
        }
    }
    printf("Поток %d завершён\n", thread_id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    cpu_set_t set;

    // Привязка к одному ядру для наглядности конкуренции
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    if (sched_setaffinity(0, sizeof(cpu_set_t), &set) == -1) {
        perror("Ошибка привязки к ядру");
        exit(1);
    }

    printf("Потоки одного процесса с разными приоритетами\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        pthread_attr_t attr;
        struct sched_param param;
        pthread_attr_init(&attr);
        
        if (i == 0) { // SCHED_FIFO, приоритет 60
            param.sched_priority = 60;
            pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
        } else if (i == 1) { // SCHED_RR, приоритет 50
            param.sched_priority = 50;
            pthread_attr_setschedpolicy(&attr, SCHED_RR);
        } else { // SCHED_OTHER, nice 0
            param.sched_priority = 0;
            pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
            setpriority(PRIO_PROCESS, 0, 0);
        }
        pthread_attr_setschedparam(&attr, &param);
        pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        
        int ret = pthread_create(&threads[i], &attr, thread_func, &thread_ids[i]);
        if (ret != 0) {
            fprintf(stderr, "Ошибка создания потока %d: %s\n", i + 1, strerror(ret));
            exit(1);
        }
        pthread_attr_destroy(&attr);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Эксперимент завершён\n");

    return 0;
}