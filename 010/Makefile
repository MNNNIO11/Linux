all: tie

tie: main.o
	g++ main.o -o tie
main.o: main.c
	g++ main.c -c -fPIC
clean:
	rm -rf *.o
	rm tie
