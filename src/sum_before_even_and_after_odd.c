#include<stdio.h>
#include<stdlib.h>
#include"sum_before_even_and_after_odd.h"
#include"index_first_even.h"
#include"index_last_odd.h"
int sum_before_even_and_after_odd(int arr[], int length)
{
 int sum_before_and_after=0;
 int first_even = index_first_even(arr,length);
 int last_odd = index_last_odd(arr,length);
 for(int i=0;i<first_even;i++)
 sum_before_and_after+=abs(arr[i]);
 for(int i=last_odd;i<length;i++)
 sum_before_and_after+=abs(arr[i]);
 return sum_before_and_after;
}
