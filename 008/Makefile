all: tlist

tlist: main.o
	g++ main.o -o tlist
main.o: main.c
	g++ main.c -c -fPIC 
clean:
	rm -rf *.o
	rm tlist
