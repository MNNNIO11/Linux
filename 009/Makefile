# My second makefile
all: piperead

piperead: main.o
	gcc main.o -o piperead
main.o: main.c
	gcc main.c -c -fPIC
clean:
	rm -rf *.o
	rm piperead
