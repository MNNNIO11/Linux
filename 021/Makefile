all: mmsort

mmsort: main.o
	gcc main.o -o mmsort
main.o: main.c
	gcc main.c -c -fPIC
clean:
	rm -rf *.o
	rm mmsort
