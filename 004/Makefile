# My second makefile
all: ProcTree

ProcTree: main.o
	gcc main.o -o ProcTree
main.o: main.c
	gcc main.c -c -fPIC
clean:
	rm -rf *.o
	rm ProcTree
