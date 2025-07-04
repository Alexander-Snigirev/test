#include <stdio.h>
#include <unistd.h>

int main(){
    printf("Пользователь: %s\n Права: %d\n", getlogin(), geteuid());
    return 0;
}