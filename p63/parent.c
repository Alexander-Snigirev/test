#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <bits/types/sigset_t.h>
#include <bits/sigaction.h>

void signal_handler(int sig) {
    printf("PID %d: Получен сигнал %d\n", getpid(), sig);
}

int main() {
    pid_t pid;
    sigset_t set;
    int sig;

    // Блокируем SIGUSR1 и SIGUSR2 для использования в sigwait
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigprocmask(SIG_BLOCK, &set, NULL);

    // Устанавливаем обработчик для SIGUSR1
    signal(SIGUSR1, signal_handler);
    printf("Родитель (PID %d): Установлен обработчик для SIGUSR1\n", getpid());

    pid = fork();
    if (pid < 0) {
        perror("Ошибка fork");
        exit(1);
    }

    if (pid == 0) { // Дочерний процесс
        printf("Потомок (PID %d): После fork\n", getpid());
        
        // Проверяем обработчик
        kill(getpid(), SIGUSR1);

        // Отправляем родителю сигнал SIGUSR2, что готовы к exec
        kill(getppid(), SIGUSR2);

        // Вызываем exec
        execl("./child", "child", (char*)NULL);
        perror("Ошибка exec");
        exit(1);
    } else { // Родительский процесс
        // Ждём SIGUSR2 от потомка
        sigwait(&set, &sig);
        if (sig == SIGUSR2) {
            printf("Родитель (PID %d): Получен SIGUSR2 от потомка, отправляю SIGUSR1\n", getpid());
            kill(pid, SIGUSR1);
            wait(NULL);
        }
    }

    return 0;
}