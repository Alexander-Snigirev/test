#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>


int main(){
    pid_t pid;
    pid = fork();
    if(pid<0){
        perror("fork failed");
        exit(1);
    }

    if(pid==0){
        printf("Дочерний процесс:\n");
        printf("PID: %d\nPPID: %d\n", getpid(), getppid());
        for(int i=0;i<5;i++){
            printf("Шаг дочернего процесса - %d\n", i+1);
            for(int j=0;j<10;j++){
                printf("Щелк %d\n", j);
                for(int i=0;i<100000000;i++){}
                //sleep(1);
            }
            //sleep(2);
            for(int i=0;i<200000000;i++){}
        }
        printf("Дочерний процесс завершен\n");
        exit(0);
    }
    else{
        printf("Родительский процесс:\nPID: %d\nPPID: %d\n", getpid(), getppid());
        for(int i=0;i<5;i++){
            printf("Шаг родительского процесса - %d\n", i+1);
            for(int j=0;j<7;j++){
                printf("Хрю %d\n", j);
                for(int i=0;i<100000000;i++){}
            }
            for(int i=0;i<200000000;i++){}
        }
        wait(NULL);
        printf("Родительский процесс завершен\n");
    }
}