#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void print_ps_state(const char* label) {
    // Фиксируем состояние процессов в файл и на экран
    printf("\n[%s] Состояние процессов:\n", label);
    system("ps -l");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <a|b|c>\n", argv[0]);
        return 1;
    }

    pid_t pid;
    char choice = argv[1][0];

    // Фиксируем состояние процессов до создания потомка
    print_ps_state("До создания потомка");

    if (choice == 'a') {
        // Ситуация а: Родитель ожидает завершения потомка
        pid = fork();
        if (pid == 0) {
            // Дочерний процесс
            printf("Потомок: PID = %d, PPID = %d\n", getpid(), getppid());
            sleep(2);  // Симуляция работы
            exit(0);
        } else {
            // Родительский процесс
            printf("Родитель: PID = %d, ожидает завершения потомка\n", getpid());
            wait(NULL);  // Родитель ожидает завершения потомка
            print_ps_state("После завершения потомка (ситуация а)");
        }

    } else if (choice == 'b') {
        // Ситуация б: Родитель завершает работу, не дождавшись потомка
        pid = fork();
        if (pid == 0) {
            // Дочерний процесс
            printf("Потомок: PID = %d, PPID = %d\n", getpid(), getppid());
            sleep(5);  // Симуляция длительной работы
            exit(0);
        } else {
            // Родительский процесс
            printf("Родитель: PID = %d завершает работу без ожидания потомка.\n", getpid());
            print_ps_state("До завершения родителя (ситуация б)");
            exit(0);
        }

    } else if (choice == 'c') {
        // Ситуация в: Родитель не ждет завершения, потомок становится зомби
        pid = fork();
        if (pid == 0) {
            // Дочерний процесс
            printf("Потомок: PID = %d, PPID = %d\n", getpid(), getppid());
            for(int i=0;i<500000000;i++){}  // Симуляция работы
            exit(0);
        } else {
            // Родительский процесс
            printf("Родитель: PID = %d завершает работу, не дождавшись потомка.\n", getpid());
            print_ps_state("До завершения родителя (ситуация в)");
            for(int i=0;i<600000000;i++){} // Даем время, чтобы потомок стал зомби
            print_ps_state("После завершения родителя (ситуация в)");
        }

    } else {
        fprintf(stderr, "Некорректный параметр. Используйте a, b или c.\n");
        return 1;
    }

    return 0;
}