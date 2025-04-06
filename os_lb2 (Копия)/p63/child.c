#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main() {
    printf("После exec (PID %d): Жду сигнал\n", getpid());
    pause();
    printf("После exec: Сигнал обработан (не должно напечататься без обработчика)\n");
    return 0;
}