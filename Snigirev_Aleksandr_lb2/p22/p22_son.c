#include<stdio.h>
#include<unistd.h>

int main(){
    printf("Потомок начал работу: PID = %d, PPID = %d\n", getpid(), getppid());
    for(int i=0;i<500000000;i++){}
    printf("Потомок завершил работу.\n");
    return 0;
}