# My second makefile
all: daemon

daemon: main.o
	gcc main.o -o daemon
main.o: main.c
	gcc main.c -c -fPIC
clean:
	rm -rf *.o
	rm daemon
