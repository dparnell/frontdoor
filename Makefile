frontdoor: main.o font.o
	gcc main.o font.o -l curses -o frontdoor

main.o: main.c
	gcc -c main.c

font.o: font.c
	gcc -c font.c