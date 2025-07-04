#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        printf("Дочерний процесс: PID = %d\n", getpid());
        for(int i=0;i<200000000;i++){}
        exit(40);
    } else {
        int status;
        pid_t terminated_pid = wait(&status);
        
        if (terminated_pid > 0) {
            if (WIFEXITED(status)) {
                printf("Процесс завершился с кодом: %d\n",  WEXITSTATUS(status));
            } else {
                printf("Процесс завершился сигналом %d\n", WTERMSIG(status));
            }
        }
    }
    return 0;
}