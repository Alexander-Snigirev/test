#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

void compute_task(char sym, FILE* ff) {
    volatile long long i;
    long long int sum=0;
    for (i = 0; i < 700000000LL; i++){
        if(i%100000000==0)
        fprintf(ff,"Процесс %c работает\n", sym);
        fflush(ff);
    }
         // Простая нагрузка
   
}

int main() {
    FILE* ff = fopen("out3.txt", "a");
    pid_t pids[4];
    struct sched_param params[4];
    struct timeval start, end;
    double time_spent;
    cpu_set_t set;

    // Привязка к одному ядру (CPU 0)
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    if (sched_setaffinity(0, sizeof(cpu_set_t), &set) == -1) {
        perror("Ошибка привязки к ядру");
        exit(1);
    }

    // Задаём приоритеты
    params[0].sched_priority = 10;  // Процесс 1
    params[1].sched_priority = 20;  // Процесс 2
    params[2].sched_priority = 30;   // Процесс 3
    params[3].sched_priority = 40;  // Процесс 4

    // Создаём все процессы
    for (int i = 0; i < 4; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            // Устанавливаем SCHED_FIFO для каждого процесса
            if (sched_setscheduler(0, SCHED_RR, &params[i]) == -1) {
                perror("Ошибка установки SCHED_RR");
                exit(1);
            }
            fprintf(ff,"Процесс %d (PID: %d) начал выполнение с приоритетом %d\n", 
                   i + 1, getpid(), params[i].sched_priority);
            
            gettimeofday(&start, NULL);
            compute_task(i+49, ff);
            gettimeofday(&end, NULL);
            
            time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
            fprintf(ff,"Процесс %d завершён. Время: %.3f секунд\n", i + 1, time_spent);
            exit(0);
        }
    }

    // Ждём завершения всех процессов
    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }
    fprintf(ff,"Все процессы завершены.\n");
    fclose(ff);
    return 0;
}