#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

void compute_task(int process_id) {
    int policy = sched_getscheduler(0);
    struct sched_param param;
    sched_getparam(0, &param);
    printf("Процесс %d (PID: %d): политика %d, приоритет %d\n", 
           process_id, getpid(), policy, param.sched_priority);
    
    volatile long long i;
    for (i = 0; i < 700000000LL; i++) {
        if (i % 100000000 == 0) {
            printf("Процесс %d работает (итерация %lld)\n", process_id, i);
            fflush(stdout);
        }
    }
    printf("Процесс %d завершён\n", process_id);
}

int main() {
    pid_t pids[3];
    cpu_set_t set;

    CPU_ZERO(&set);
    CPU_SET(0, &set);
    if (sched_setaffinity(0, sizeof(cpu_set_t), &set) == -1) {
        perror("Ошибка привязки к ядру");
        exit(1);
    }

    printf("Независимые процессы с разными приоритетами\n");
    for (int i = 0; i < 3; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            struct sched_param param;
            if (i == 0) { // SCHED_FIFO, приоритет 60
                param.sched_priority = 60;
                sched_setscheduler(0, SCHED_FIFO, &param);
            } else if (i == 1) { // SCHED_RR, приоритет 50
                param.sched_priority = 50;
                sched_setscheduler(0, SCHED_RR, &param);
            } else { // SCHED_OTHER, nice 0
                param.sched_priority = 0;
                sched_setscheduler(0, SCHED_OTHER, &param);
                setpriority(PRIO_PROCESS, 0, 0);
            }
            compute_task(i + 1);
            exit(0);
        }
    }
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }
    printf("Эксперимент завершён\n");

    return 0;
}