all: namesp

namesp: main.o
	gcc main.o -o namesp
main.o: main.c
	gcc main.c -c -fPIC
clean:
	rm -rf *.o
	rm namesp
