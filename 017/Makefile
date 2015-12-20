all: summator

summator: main.o
	g++ main.o -o summator
main.o: main.c
	g++ main.c -c -fPIC
clean:
	rm -rf *.o
	rm summator
