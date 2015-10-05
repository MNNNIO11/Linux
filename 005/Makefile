# My second makefile
all: MyShell

MyShell: main.o
	gcc main.o -o MyShell
main.o: main.c
	gcc main.c -c -fPIC
clean:
	rm -rf *.o
	rm MyShell
