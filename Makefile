main: main.o
	gcc -o main main.o

main.o: main.c
	gcc -c main.c

make run:
	make; make start; make clean;

start:
	./main

clear:
	rm *.o main