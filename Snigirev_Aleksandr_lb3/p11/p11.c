/*1.1. Создайте программу с порождением двух нитей так, чтобы управление второй нитью 
    осуществлялось посредством сигнала SIGUSR1 из первой нити
    (после некоторого заданного интервала времени выполнения обеих нитей удалите вторую 
    нить отправкой сигнала из первой нити).*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>
#include <errno.h>

#define LOOP_ITERATIONS 100000000
#define SIGNAL_DELAY_SECONDS 5
#define NUM_THREADS 2

typedef struct {
    int thread_num;
} ThreadArg;

pid_t gettid() {
    return syscall(SYS_gettid);
}

// Обработчик сигнала
void signal_handler(int sig) {
    pid_t tid = gettid();
    if (sig == SIGUSR1) {
        printf("Signal %d (SIGUSR1) received by TID=%ld\n", sig, (long)tid);
        fflush(stdout);
        
    }
}

// Функция для отправки SIGUSR1 второй нити
void send_signal_to_thread(union sigval sv) {
    pthread_t* t2 = (pthread_t*)sv.sival_ptr;
    printf("Timer expired, sending SIGUSR1 to second thread\n");
    fflush(stdout);
    if (pthread_kill(*t2, SIGUSR1) != 0) {
        perror("pthread_kill failed");
    }
}

// Функция для нити
void* thread_func(void* arg) {
    ThreadArg* data = (ThreadArg*)arg;
    pid_t tid = gettid();
    int thread_num = data->thread_num;

    printf("Thread %d started, TID=%ld\n", thread_num, (long)tid);
    fflush(stdout);

    for (int i = 0; i < 5; i++) {
        printf("Thread %d, TID=%ld: iteration %d\n", thread_num, (long)tid, i);
        fflush(stdout);
        for (int j = 0; j < LOOP_ITERATIONS; j++); // Имитация работы
        sleep(2);
    }

    printf("Thread %d, TID=%ld completed\n", thread_num, (long)tid);
    fflush(stdout);
    return NULL;
}

int main() {
    pid_t main_pid = getpid();
    pid_t main_tid = gettid();
    printf("Main process PID=%ld, Main TID=%ld\n", (long)main_pid, (long)main_tid);

    // Устанавливаем обработчик для SIGUSR1
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction for SIGUSR1 failed");
        exit(EXIT_FAILURE);
    }

    // Создаём нити
    pthread_t threads[NUM_THREADS];
    ThreadArg args[NUM_THREADS];

    printf("Creating threads...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].thread_num = i + 1;
        if (pthread_create(&threads[i], NULL, thread_func, &args[i]) != 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    // Настраиваем таймер для отправки SIGUSR1 второй нити
    timer_t timerid;
    struct sigevent sev;
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = send_signal_to_thread;
    sev.sigev_value.sival_ptr = &threads[1]; // Дескриптор второй нити
    sev.sigev_notify_attributes = NULL;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("timer_create failed");
        exit(EXIT_FAILURE);
    }

    struct itimerspec its;
    its.it_value.tv_sec = SIGNAL_DELAY_SECONDS;///////////////////////////////////////////////////////
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0; // Однократный таймер
    its.it_interval.tv_nsec = 0;
    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime failed");
        exit(EXIT_FAILURE);
    }

    // Ожидаем завершения нитей
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join failed");
        }
    }
    printf("All threads completed.\n");
    printf("Main process exiting normally.\n");

    // Очищаем таймер
    timer_delete(timerid);
    return 0;
}