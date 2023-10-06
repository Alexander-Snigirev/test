#include<stdio.h>
#include<stdlib.h>
#include"sum_between_even_odd.h"
#include"index_first_even.h"
#include"index_last_odd.h"
int sum_between_even_odd(int arr[],int length)
{
 int a=0;
 int first = index_first_even(arr,length);
 int last = index_last_odd(arr,length);
 for(int i=first;i<last;i++)
 a+=abs(arr[i]);
 return a;
}
