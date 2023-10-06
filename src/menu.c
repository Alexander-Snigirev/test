#include <stdio.h>
#include <stdlib.h>
#include"index_first_even.h"
#include"index_last_odd.h"
#include"sum_before_even_and_after_odd.h"
#include"sum_between_even_odd.h"
#define N 100 
int main()
{
 int value;
 scanf("%d",&value);
 int arr[N];
 int i=0;
 while(scanf("%d", &arr[i])==1)
 i++;
 switch(value)
 {
  case 0:
   printf("%d\n", index_first_even(arr,i));
   return 0;
  case 1:
   printf("%d\n", index_last_odd(arr,i));
   return 0;
  case 2:
   printf("%d\n", sum_between_even_odd(arr,i));
   return 0;
  case 3:
   printf("%d\n",sum_before_even_and_after_odd(arr,i));
   return 0;
  default:
      printf("Данные некорректны\n");
      return 0;
 }
}
