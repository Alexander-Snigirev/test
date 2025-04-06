#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/resource.h>

int main() {
    int policy = sched_getscheduler(0);
    struct sched_param param;
    sched_getparam(0, &param);
    int nice_val = getpriority(PRIO_PROCESS, 0);
    printf("После exec: PID %d, политика %d, приоритет %d, nice %d\n", 
           getpid(), policy, param.sched_priority, nice_val);
    return 0;
}