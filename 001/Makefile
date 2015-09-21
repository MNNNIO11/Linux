# My second makefile
all: HelloWorld

libhello.so: HelloWorld.c 
	gcc  -shared -C HelloWorld.c -o libhello.so -fPIC

HelloWorld: libhello.so
	gcc main.c -L. -lhello -o HelloWorld -fPIC

HelloWorld.c: HelloWorld.h

clean:
	rm -rf *.o
	rm libhello.so
	rm HelloWorld
