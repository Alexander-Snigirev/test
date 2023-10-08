#include <stdio.h>
#include <stdlib.h>
#include"switch_menu.h"
#define N 100 
int main()
{
 int value;
 scanf("%d",&value);
 int arr[N];
 int length=0;
 while(scanf("%d", &arr[length])==1)
 length++;
 switch_menu(value,arr,length);
}
