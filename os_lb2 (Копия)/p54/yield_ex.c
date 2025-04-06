#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

void compute_task(int process_id, FILE* ff, int use_yield) {
    volatile long long i;
    for (i = 0; i < 7000000000LL; i++) {
        if (i % 100000000 == 0) {
            fprintf(ff, "Процесс %d работает (итерация %lld)\n", process_id, i);
            fflush(ff);
            if (use_yield && i == 300000000) { // Уступаем на середине
                fprintf(ff, "Процесс %d вызывает sched_yield()\n", process_id);
                fflush(ff);
                sched_yield();
            }
        }
    }
    fprintf(ff, "Процесс %d завершён\n", process_id);
    fflush(ff);
}

int main() {
    FILE* ff = fopen("yield_test.txt", "a");
    pid_t pids[2];
    struct sched_param param;
    

    // Одинаковый приоритет для SCHED_RR
    param.sched_priority = 60;

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
            compute_task(i + 1, ff, (i == 0)); // Первый процесс вызывает yield
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