#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void print_ps_state(const char *label) {
    // Фиксируем состояние процессов
    printf("\n[%s] Состояние процессов:\n", label);
    system("ps -s >> file");  // Отображаем таблицу процессов в файл
}

int main() {
    pid_t pid1, pid2, pid3;

    // Порождение сыновей
    pid1 = fork();
    if (pid1 == 0) {
        execl("./son1", "son1", NULL);
        exit(0);
    }

    pid2 = fork();
    if (pid2 == 0) {
        execl("./son2", "son2", NULL);
        exit(0);
    }

    pid3 = fork();
    if (pid3 == 0) {
        execl("./son3", "son3", NULL);
        exit(0);
    }

    // Ожидание дочерних процессов
    sleep(1);

    // Отображаем состояние до отправки сигналов
    print_ps_state("До отправки сигналов");

    // Генерация сигналов
    printf("Отправка SIGUSR1 процессу son1\n");
    kill(pid1, SIGUSR1);

    printf("Отправка SIGUSR1 процессу son2\n");
    kill(pid2, SIGUSR1);

    printf("Отправка SIGUSR1 процессу son3\n");
    kill(pid3, SIGUSR1);

    print_ps_state("После отправки сигналов");


    // Ожидание завершения всех потомков
    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}
