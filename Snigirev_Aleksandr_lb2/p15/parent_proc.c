#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>


int main(){
    __pid_t pid;
    const char* programs[] = {"./proc1","./proc2","./proc3"};
    int num = sizeof(programs)/sizeof(programs[0]);

    for(int i = 0;i<num;i++){
        pid = fork();
        if(!pid){
            execl(programs[i], programs[i], (char*)NULL);
            perror("exec failed");
            exit(1);
        }
        else if(pid<0){
            perror("fork failed");
        }
    }

    for(int i = 0; i < num;i++){
        wait(NULL);

    }
    return 0;
}