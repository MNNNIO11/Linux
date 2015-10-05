# My second makefile
all: MyKill

MyKill: main.o
	gcc main.o -o MyKill
main.o: main.c
	gcc main.c -c -fPIC
clean:
	rm -rf *.o
	rm MyKill
