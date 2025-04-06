#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <time.h>

void compute_task(int process_id, FILE* ff) {
    struct timespec start, current;
    long long iterations = 0;
    const long long max_iterations = 1000000000LL; // Долгая задача
    
    clock_gettime(CLOCK_MONOTONIC, &start); // Начало времени
    while (iterations < max_iterations) {
        iterations++;
        if (iterations % 10000000 == 0) { // Проверяем каждые 10M итераций
            clock_gettime(CLOCK_MONOTONIC, &current);
            double elapsed = (current.tv_sec - start.tv_sec) + 
                             (current.tv_nsec - start.tv_nsec) / 1e9;
            fprintf(ff, "Процесс %d: итерация %lld, время %.6f сек\n", 
                    process_id, iterations, elapsed);
            fflush(ff);
        }
    }
}

int main() {
    FILE* ff = fopen("quantum_test.txt", "a");
    pid_t pids[2]; // Два процесса для конкуренции
    struct sched_param param;


    // Одинаковый приоритет для SCHED_RR
    param.sched_priority = 10;

    // Создаём два процесса
    for (int i = 0; i < 2; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
                perror("Ошибка установки SCHED_RR");
                exit(1);
            }
            fprintf(ff, "Процесс %d (PID: %d) начал с приоритетом %d\n", 
                    i + 1, getpid(), param.sched_priority);
            fflush(ff);
            compute_task(i + 1, ff);
            exit(0);
        }
    }

    for (int i = 0; i < 2; i++) {
        wait(NULL);
    }
    fprintf(ff, "Все процессы завершены.\n");
    fclose(ff);
    return 0;
}