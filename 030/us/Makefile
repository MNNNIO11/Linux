# My second makefile
all: us

us: main.o
	gcc main.o -o us
main.o: main.c
	gcc main.c -c -fPIC
clean:
	rm -rf *.o
	rm us
