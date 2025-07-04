#include<stdio.h>
#include<unistd.h>
#include<sched.h>
#include<sys/types.h>
#include<sys/syscall.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/resource.h>
#include<errno.h>
#include<signal.h>

pid_t gettid(){
    return syscall(SYS_gettid);
}

int main(){
    pid_t pid = getpid();
    pid_t tid = gettid();
    int policy = sched_getscheduler(0);
    struct sched_param param;
    FILE* ff = fopen("output5.txt", "a");
    if(policy ==-1){
        perror("sched_getscheduler failed");
        return 1;
    }
    if(sched_getparam(pid, &param)==-1) {
        perror("sched_getparam failed");
        return 1;
    }
    fprintf(ff,"PID = %d, TID = %d, PRI = %d\n", pid, tid, 19-getpriority(PRIO_PROCESS, 0));
    switch (policy){
        case SCHED_OTHER:
        fprintf(ff, "Scheduling policy: SCHED_OTHER\n");
        break;
        case SCHED_FIFO:
        fprintf(ff, "Scheduling policy: SCHED_FIFO\n");
        break;
        case SCHED_RR:
        fprintf(ff,"Scheduling policy: SCHED_RR\n");
        break;
        default:
        fprintf(ff,"Unknown policy (%d)\n", policy);
    }
    sleep(10);
    int nice = getpriority(PRIO_PROCESS, 0);
    fprintf(ff,"Nice: %d\n", nice);
    fclose(ff);
}