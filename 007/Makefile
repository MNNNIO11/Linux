all: summator

summator: main.o
	g++ main.o -pthread -o summator
main.o: main.c
	g++ main.c -c -fPIC -pthread
clean:
	rm -rf *.o
	rm summator
