/*3.1. Используя функцию sigaction(), продемонстрируйте возможности управления линейкой сигналов,
включая собственные обработчики и маскирование для разных сигналов,
а также вариативность, предоставляемую структурами sigaction (act / oldact).*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <errno.h>
#include <string.h>

#define NUM_THREADS 2
#define LOOP_ITERATIONS 5

typedef struct {
    int thread_num;
} ThreadArg;

pid_t gettid() {
    return syscall(SYS_gettid);
}


void sigusr1_handler(int sig) {
    pid_t tid = gettid();
    printf("SIGUSR1 handler started for TID=%ld\n", (long)tid);
    printf("Signal %d (SIGUSR1) received by TID=%ld\n", sig, (long)tid);
    fflush(stdout);
    sleep(1); 
    printf("SIGUSR1 handler finished for TID=%ld\n", (long)tid);
    fflush(stdout);
}


void sigusr2_handler(int sig) {
    pid_t tid = gettid();
    printf("SIGUSR2 handler started for TID=%ld\n", (long)tid);
    printf("Signal %d (SIGUSR2) received by TID=%ld\n", sig, (long)tid);
    fflush(stdout);
    sleep(1); 
    printf("SIGUSR2 handler finished for TID=%ld\n", (long)tid);
    fflush(stdout);
}


void sigterm_handler(int sig) {
    pid_t tid = gettid();
    printf("SIGTERM handler started for TID=%ld\n", (long)tid);
    printf("Signal %d (SIGTERM) received by TID=%ld, terminating thread\n", sig, (long)tid);
    fflush(stdout);
    pthread_exit(NULL);
}


void* thread_func(void* arg) {
    ThreadArg* data = (ThreadArg*)arg;
    pid_t tid = gettid();
    int thread_num = data->thread_num;

    printf("Thread %d started, TID=%ld\n", thread_num, (long)tid);
    fflush(stdout);

    
    struct sigaction sa_usr1, sa_usr2, sa_term, old_term;

    
    sa_usr1.sa_handler = sigusr1_handler;
    sa_usr1.sa_flags = 0;
    sigemptyset(&sa_usr1.sa_mask);
    sigaddset(&sa_usr1.sa_mask, SIGUSR2); 
    if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
        perror("sigaction SIGUSR1 failed");
        pthread_exit(NULL);
    }

    
    sa_usr2.sa_handler = sigusr2_handler;
    sa_usr2.sa_flags = SA_RESTART;
    sigemptyset(&sa_usr2.sa_mask);
    sigaddset(&sa_usr2.sa_mask, SIGUSR1); 
    if (sigaction(SIGUSR2, &sa_usr2, NULL) == -1) {
        perror("sigaction SIGUSR2 failed");
        pthread_exit(NULL);
    }

    
    sa_term.sa_handler = sigterm_handler;
    sa_term.sa_flags = 0;
    sigemptyset(&sa_term.sa_mask);
    if (sigaction(SIGTERM, &sa_term, &old_term) == -1) {
        perror("sigaction SIGTERM failed");
        pthread_exit(NULL);
    }

    
    for (int i = 0; i < LOOP_ITERATIONS; i++) {
        printf("Thread %d, TID=%ld: iteration %d\n", thread_num, (long)tid, i);
        fflush(stdout);
        sleep(2); 
    }

    printf("Thread %d, TID=%ld completed\n", thread_num, (long)tid);
    fflush(stdout);

    
    if (sigaction(SIGTERM, &old_term, NULL) == -1) {
        perror("sigaction restore SIGTERM failed");
    }

    return NULL;
}

int main() {
    pid_t main_pid = getpid();
    pid_t main_tid = gettid();
    printf("Main process PID=%ld, Main TID=%ld\n", (long)main_pid, (long)main_tid);

    
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

    sleep(1);    
    printf("Main: sending SIGUSR1 to thread 1\n");
    if (pthread_kill(threads[0], SIGUSR1) != 0) {
        perror("pthread_kill SIGUSR1 failed");
    }
    
    sleep(2);
    
    printf("Main: sending SIGUSR2 to thread 1\n");
    if (pthread_kill(threads[0], SIGUSR2) != 0) {
        perror("pthread_kill SIGUSR2 failed");
    }
    printf("Main: sending SIGUSR2 to thread 2\n");
    if (pthread_kill(threads[1], SIGUSR2) != 0) {
        perror("pthread_kill SIGUSR2 failed");
    }
    sleep(3);

    printf("Main: sending SIGTERM to thread 2\n");
    if (pthread_kill(threads[1], SIGTERM) != 0) {
        perror("pthread_kill SIGTERM failed");
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join failed");
        }
    }
    printf("All threads completed.\n");
    printf("Main process exiting normally.\n");

    return 0;
}