#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void signal_handler(int sig) {
    printf("son3: Получен сигнал %d, перехватываем его\n", sig);
}

int main() {
    // Устанавливаем обработчик для SIGUSR1
    signal(SIGUSR1, signal_handler);
    printf("son3: PID = %d, PPID = %d\n", getpid(), getppid());

    while (1) {
        //sleep(1);  // Бесконечный цикл, будем перехватывать сигнал
        for(int i=0;i<100000000;i++){}
    }

    return 0;
}