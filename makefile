app : main.o draw.o
	gcc -o app main.o draw.o -lm -lX11

main.o : main.c draw.h
	gcc -c main.c -lX11 -lm

draw.o : draw.c draw.h
	gcc -c draw.c -lm -I.

