#include<stdio.h>
#include<stdlib.h>
#include"index_last_odd.h"
int index_last_odd(int arr[], int length)
{
 int last_odd;
 for(int i=0;i<length;i++)
 {
  if(abs(arr[i])%2==1)
  last_odd = i;
 }
 return last_odd;
}
