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
#include <inttypes.h>

int main(){
    FILE* ff = fopen("out2.txt", "a");
    struct sched_param param;
    struct timeval start, end;
    double time_spent;

    // Устанавливаем параметры для SCHED_FIFO с приоритетом 10
    
    param.sched_priority = 10;
    while(sched_setscheduler(0, SCHED_RR ,&param) == 0) {
        param.sched_priority--;
    }
    fprintf(ff, "Minimal nice = %d\n", param.sched_priority+1);

    param.sched_priority+=1;
    while(sched_setscheduler(0, SCHED_RR, &param) == 0) {
        param.sched_priority++;
    }
    fprintf(ff, "Maximal nice = %d\n", param.sched_priority-1);

    fclose(ff);
}