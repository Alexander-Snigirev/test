all: menu.o index_first_even.o index_last_odd.o sum_between_even_odd.o sum_before_even_and_after_odd.o switch_menu.o
	gcc menu.o index_first_even.o index_last_odd.o sum_between_even_odd.o sum_before_even_and_after_odd.o switch_menu.o -o menu

menu.o: menu.c switch_menu.h
	gcc -c -std=c99 menu.c

switch_menu.o: switch_menu.c switch_menu.h
	gcc -c -std=c99 switch_menu.c

index_first_even.o: index_first_even.c index_first_even.h
	gcc -c -std=c99 index_first_even.c

index_last_odd.o: index_last_odd.c index_last_odd.h
	gcc -c -std=c99 index_last_odd.c
       	

sum_between_even_odd.o: sum_between_even_odd.c sum_between_even_odd.h
	gcc -c -std=c99 sum_between_even_odd.c

sum_before_even_and_after_odd.o: sum_before_even_and_after_odd.c sum_before_even_and_after_odd.h
	gcc -c -std=c99 sum_before_even_and_after_odd.c


