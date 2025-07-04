#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    // Игнорируем сигнал SIGUSR1
    signal(SIGUSR1, SIG_IGN);
    printf("son2: PID = %d, PPID = %d\n", getpid(), getppid());

    while (1) {
        for(int i=0;i<100000000;i++){}  // Бесконечный цикл, сигнал будет проигнорирован
    }

    return 0;
}