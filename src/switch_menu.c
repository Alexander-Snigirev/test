#include<stdio.h>
#include"index_first_even.h"
#include"index_last_odd.h"
#include"sum_between_even_odd.h"
#include"sum_before_even_and_after_odd.h"
int switch_menu(int value, int arr[], int length)
{
	switch(value){
		case 0:
			printf("%d\n", index_first_even(arr, length));
			break;
		case 1:
			printf("%d\n", index_last_odd(arr, length));
			break;
		 case 2:
                        printf("%d\n", sum_between_even_odd(arr, length));
                        break;
		 case 3: 
                        printf("%d\n", sum_before_even_and_after_odd(arr, length));
                        break;
		default:
			printf("Данные некорректны\n");
			break;
}
}