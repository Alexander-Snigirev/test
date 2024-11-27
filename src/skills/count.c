#include<stdio.h>
int main(){
    char sym;
    int count=0;
    while(1){
        sym = getchar();
        if(sym=='#'){
            break;
        }
        count++;
    }
    printf("%d\n", count);
}