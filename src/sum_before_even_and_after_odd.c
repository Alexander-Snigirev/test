#include<stdio.h>
#include<stdlib.h>
#include"sum_before_even_and_after_odd.h"
#include"index_first_even.h"
#include"index_last_odd.h"
int sum_before_even_and_after_odd(int arr[], int length)
{
 int a=0;
 int first = index_first_even(arr,length);
 int last = index_last_odd(arr,length);
 for(int i=0;i<first;i++)
 a+=abs(arr[i]);
 for(int i=last;i<length;i++)
 a+=abs(arr[i]);
 return a;
}
