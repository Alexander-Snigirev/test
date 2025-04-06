#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>

#define NUM_THREADS 3

void* thread_func(void* arg) {
    int thread_id = *(int*)arg;
    int policy;
    struct sched_param param;
    
    // Получаем текущую политику и приоритет
    pthread_getschedparam(pthread_self(), &policy, &param);
    printf("Поток %d (TID: %ld): политика %d, приоритет %d\n", 
           thread_id, pthread_self(), policy, param.sched_priority);
    
    //3адача для демонстрации конкуренции
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


    // Эксперимент 1: Политика по умолчанию
    printf("Эксперимент 1: Политика по умолчанию\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        if (pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]) != 0) {
            perror("Ошибка создания потока");
            exit(1);
        }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Эксперимент 1 завершён\n\n");

    // Эксперимент 2: Изменение политики для всех потоков (SCHED_RR)
    printf("Эксперимент 2: Все потоки с SCHED_RR, приоритет 50\n");
    pthread_attr_t attr;
    struct sched_param param;
    pthread_attr_init(&attr);
    param.sched_priority = 50;
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED); // Явное задание политики

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        if (pthread_create(&threads[i], &attr, thread_func, &thread_ids[i]) != 0) {
            perror("Ошибка создания потока");
            exit(1);
        }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_attr_destroy(&attr);
    printf("Эксперимент 2 завершён\n\n");

    // Эксперимент 3: Разные политики для потоков
    printf("Эксперимент 3: Поток 1 - SCHED_RR, остальные - SCHED_OTHER\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        pthread_attr_t custom_attr;
        pthread_attr_init(&custom_attr);
        
        if (i == 0) { // Только первый поток с SCHED_RR
            param.sched_priority = 50;
            pthread_attr_setschedpolicy(&custom_attr, SCHED_RR);
            pthread_attr_setschedparam(&custom_attr, &param);
            pthread_attr_setinheritsched(&custom_attr, PTHREAD_EXPLICIT_SCHED);
        } else { // Остальные с SCHED_OTHER
            param.sched_priority = 0;
            pthread_attr_setschedpolicy(&custom_attr, SCHED_OTHER);
            pthread_attr_setschedparam(&custom_attr, &param);
            pthread_attr_setinheritsched(&custom_attr, PTHREAD_EXPLICIT_SCHED);
        }
        
        if (pthread_create(&threads[i], &custom_attr, thread_func, &thread_ids[i]) != 0) {
            perror("Ошибка создания потока");
            exit(1);
        }
        pthread_attr_destroy(&custom_attr);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Эксперимент 3 завершён\n");

    return 0;
}