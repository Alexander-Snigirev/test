#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define STACK_SIZE (1024 * 1024) // Стек для потомка

void signal_handler(int sig) {
    printf("PID %d: Получен сигнал %d\n", getpid(), sig);
}

int child_func(void* arg) {
    int fd = *(int*)arg;

    // Информация о планировании
    int policy = sched_getscheduler(0);
    struct sched_param param;
    sched_getparam(0, &param);
    printf("Потомок (PID %d): политика %d, приоритет %d\n", 
           getpid(), policy, param.sched_priority);

    // Пишем в файл
    write(fd, "Потомок: запись\n", 16);

    // Проверяем обработчик сигнала
    kill(getpid(), SIGUSR1);

    return 0;
}

int main() {
    char* stack = malloc(STACK_SIZE);
    if (!stack) {
        perror("Ошибка выделения стека");
        exit(1);
    }
    char* stack_top = stack + STACK_SIZE; // Верхняя граница стека

    pid_t pid;
    int fd;

    // Устанавливаем политику и приоритет
    struct sched_param param;
    param.sched_priority = 50;
    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        perror("Ошибка установки SCHED_RR");
        exit(1);
    }

    // Устанавливаем обработчик сигнала
    signal(SIGUSR1, signal_handler);

    // Открываем файл
    fd = open("clone_test.txt", O_RDWR | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        exit(1);
    }
    write(fd, "Родитель: до clone\n", 19);

    printf("Родитель (PID %d): политика %d, приоритет %d\n", 
           getpid(), sched_getscheduler(0), param.sched_priority);

    // Эксперимент 1: clone как fork (полная копия)
    printf("\nЭксперимент 1: clone без разделения\n");
    pid = clone(child_func, stack_top, SIGCHLD, &fd);
    if (pid == -1) {
        perror("Ошибка clone в эксперименте 1");
        exit(1);
    }
    waitpid(pid, NULL, 0);

    // Эксперимент 2: clone с разделением файлов и сигналов
    printf("\nЭксперимент 2: clone с CLONE_FILES | CLONE_SIGHAND\n");
    pid = clone(child_func, stack_top, CLONE_FILES | CLONE_SIGHAND | CLONE_VM | SIGCHLD, &fd);
    if (pid == -1) {
        perror("Ошибка clone в эксперименте 2");
        exit(1);
    }
    waitpid(pid, NULL, 0);

    // Пишем из родителя после clone
    write(fd, "Родитель: после clone\n", 22);
    close(fd);
    free(stack);

    return 0;
}