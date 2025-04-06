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
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

#define STACK_SIZE (1024 * 1024)
#define ITERATIONS 10000000000 // Много квантов

void compute_task(){
    long long i,sum=0,fact;
    for(i=0;i<ITERATIONS;i++){
        sum+=i;
    }
}


int main(){
    pid_t pid1,pid2,pid3,pid4;
    struct timeval start,end;
    double time_spent;
    FILE* ff = fopen("out.txt", "a");
    pid1 = fork();
    if(pid1==0){
        int priority = getpriority(PRIO_PROCESS,0);
        fprintf(ff, "Дочерний процесс 1 (PID: %d)\nnice = %d\n", getpid(), priority);
        gettimeofday(&start, NULL);
        compute_task();
        gettimeofday(&end, NULL);
        time_spent = (end.tv_sec - start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
        fprintf(ff, "Дочерний процесс 1 завершен. Время выполнения: %.3f секунд\n", time_spent);
        exit(0);
    }

    pid2 = fork();
    if(pid2==0){
        nice(20);
        int priority = getpriority(PRIO_PROCESS,0);
        fprintf(ff, "Дочерний процесс 2 (PID: %d)\nnice = %d\n", getpid(), priority);
        gettimeofday(&start, NULL);
        compute_task();
        gettimeofday(&end, NULL);
        time_spent = (end.tv_sec - start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
        fprintf(ff, "Дочерний процесс 2 завершен. Время выполнения: %.3f секунд\n", time_spent);
        exit(0);
    }
    pid3 = fork();
    if(pid3==0){
        nice(20);
        int priority = getpriority(PRIO_PROCESS,0);
        fprintf(ff, "Дочерний процесс 3 (PID: %d)\nnice = %d\n", getpid(), priority);
        gettimeofday(&start, NULL);
        compute_task();
        gettimeofday(&end, NULL);
        time_spent = (end.tv_sec - start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
        fprintf(ff, "Дочерний процесс 3 завершен. Время выполнения: %.3f секунд\n", time_spent);
        exit(0);
    }
    pid4 = fork();
    if(pid4==0){
        nice(20);
        int priority = getpriority(PRIO_PROCESS,0);
        fprintf(ff, "Дочерний процесс 4 (PID: %d)\nnice = %d\n", getpid(), priority);
        gettimeofday(&start, NULL);
        compute_task();
        gettimeofday(&end, NULL);
        time_spent = (end.tv_sec - start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
        fprintf(ff, "Дочерний процесс 4 завершен. Время выполнения: %.3f секунд\n", time_spent);
        exit(0);
    }
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    fprintf(ff, "Дочерние процессы завершены\n");
    fclose(ff);
    return 0;

}