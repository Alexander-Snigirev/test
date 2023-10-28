#include<stdio.h>
#include<stdlib.h>
#include"sum_between_even_odd.h"
#include"index_first_even.h"
#include"index_last_odd.h"
int sum_between_even_odd(int arr[],int length)
{
 int sum=0;
 int first_even = index_first_even(arr,length);
 int last_odd = index_last_odd(arr,length);
 for(int i=first_even;i<last_odd;i++)
 sum+=abs(arr[i]);
 return sum;
}
