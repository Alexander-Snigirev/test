#include<stdio.h>
#include<stdlib.h>
#include"index_last_odd.h"
int index_last_odd(int arr[], int length)
{
 int a;
 for(int i=0;i<length;i++)
 {
  if(abs(arr[i])%2==1)
  a = i;
 }
 return a;
}
