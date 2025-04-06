#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid1 = fork();
    if (pid1 == 0) {
        printf("Дочерний процесс 1: PID = %d\n", getpid());
        for(int i=0;i<200000000;i++){}
        exit(10);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        printf("Дочерний процесс 2: PID = %d\n", getpid());
        for(int i=0;i<300000000;i++){}
        exit(20);
    }

    int status;
    printf("Ожидаем завершение дочернего процесса 1\n");
    waitpid(pid1, &status, 0);
    if (WIFEXITED(status)) {
        printf("Процесс 1 завершён с кодом: %d\n", WEXITSTATUS(status));
    }

    printf("Ожидаем завершение дочернего процесса 2\n");
    waitpid(pid2, &status, 0);
    if (WIFEXITED(status)) {
        printf("Процесс 2 завершён с кодом: %d\n", WEXITSTATUS(status));
    }

    return 0;
}