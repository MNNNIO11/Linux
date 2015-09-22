#include "HelloWorld.h"
#include <dlfcn.h>
int main ()
{
	void *ext_library; //handler for ext lib
	void (*myprint)();

	ext_library=dlopen("./libdl.so",RTLD_LAZY);
	if (!ext_library){
	    return 1;
	}
	myprint=dlsym(ext_library, "print_hello");
	(*myprint)();
	dlclose(ext_library);
}
