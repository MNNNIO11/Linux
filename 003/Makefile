all: HelloWorld

libdl.so: HelloWorld.c 
	gcc  -shared -C HelloWorld.c -o libdl.so -fPIC

HelloWorld: libdl.so
	gcc main.c -ldl -o HelloWorld -fPIC

HelloWorld.c: HelloWorld.h

clean:
	rm libdl.so
	rm HelloWorld
