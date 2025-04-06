#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <bits/sigaction.h>
#include <bits/types/sigset_t.h>

int main() {
    sigset_t set;
    int sig;

    // Блокируем SIGUSR1 и ждём его
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);

    printf("После exec (PID %d): Жду SIGUSR1\n", getpid());
    sigwait(&set, &sig);
    printf("После exec: Получен SIGUSR1, но обработчик сбросился\n");
    return 0;
}