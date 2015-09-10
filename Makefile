# My first makefile
hello: hello.o
	gcc -o hello HelloWorld.o
hello.o: HelloWorld.c
	gcc -c HelloWorld.c
clean:
	rm -rf *.o
