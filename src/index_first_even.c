#include<stdio.h>
#include"index_first_even.h"
int index_first_even(int arr[],int length)
{
 for(int i=0;i<length;i++)
 {
  if(arr[i]%2==0)
  return i;
 }
}
