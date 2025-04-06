#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void signal_handler(int sig) {
    printf("PID %d: Получен сигнал %d\n", getpid(), sig);
}

int main() {
    pid_t pid;

    signal(SIGUSR1, signal_handler);
    printf("Родитель (PID %d): Установлен обработчик для SIGUSR1\n", getpid());

    pid = fork();
    if (pid < 0) {
        perror("Ошибка fork");
        exit(1);
    }
    if (pid == 0) { 
        printf("Потомок (PID %d): После fork\n", getpid());
        kill(getpid(), SIGUSR1);
        sleep(1); 
        execl("./child_signal", "child_signal", (char*)NULL);
        perror("Ошибка exec");
        exit(1);
    } else { 
        sleep(2); 
        printf("Родитель (PID %d): Отправляю SIGUSR1 потомку\n", getpid());
        kill(pid, SIGUSR1);
        wait(NULL);
    }

    return 0;
}