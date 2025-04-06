#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void signal_handler(int sig) {
    printf("son1: Получен сигнал %d, по умолчанию процесс завершится\n", sig);
}

int main() {
    // Устанавливаем обработчик для SIGUSR1
    signal(SIGUSR1, signal_handler);
    printf("son1: PID = %d, PPID = %d\n", getpid(), getppid());

    while (1) {
        for(int i=0;i<100000000;i++){}  // Бесконечный цикл, пока не получим сигнал
    }

    return 0;
}