# My second makefile
all: pwd_search

pwd_search: main.o
	gcc main.o -o pwd_search
main.o: main.c
	gcc main.c -c -fPIC
clean:
	rm -rf *.o
	rm pwd_search
