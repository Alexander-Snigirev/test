#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>

void print_sched_info(const char* process) {
    int policy = sched_getscheduler(0);
    struct sched_param param;
    sched_getparam(0, &param);
    int nice_val = getpriority(PRIO_PROCESS, 0);
    printf("%s: PID %d,  %d,  %d, nice %d\n", 
           process, getpid(), policy, param.sched_priority, nice_val);
}

int main() {
    struct sched_param param;
    pid_t pid;

 
    param.sched_priority = 50;
    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        perror("Ошибка SCHED_RR");
        exit(1);
    }
    print_sched_info("Родитель до fork");

    pid = fork();
    if (pid < 0) {
        perror("Ошибка fork");
        exit(1);
    }

    if (pid == 0) { //  
        print_sched_info("Потомок после fork");

        //  exec  ,    
        execl("./child_exec", "child_exec", (char*)NULL);
        perror("Ошибка exec");
        exit(1);
    } else { //  
        wait(NULL);
        print_sched_info("Родитель после fork");
    }

    return 0;
}