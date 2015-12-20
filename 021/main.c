#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
int comp(const void *arg1, const void *arg2)
{
return *(char *)arg1 - *(char *)arg2;
}
int main (int argc, char* const argv[])
{
struct stat _stat;
int size_f;
int fd;
if(argc != 3)
{
  printf ("Must be 2 args: ./mmsort filename sortsize\n");
  exit (1);
}

fd = open (argv[1], O_RDWR);
if (fd == -1){
 printf  ("Can't open file\n");
 exit (1);
}

size_f = atoi (argv[2]);
if ((size_f > 256)||(size_f < 1)) {
printf ("Size must be from 1 to 256\n");
exit (1);
}
if (fstat (fd, &_stat) == -1){
  printf ("Error file status \n");
  exit (1);
}
//printf ("stat.st_size =%d\n",(int)_stat.st_size);
//printf ("sizeof =%d\n",(int)sizeof(fd));

if (_stat.st_size <= size_f) {
 printf ("Too large sort size. Sort size will be equal to the size of the file.\n");
 size_f = _stat.st_size;
}
void *_mmap = mmap(0,_stat.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd, 0);
if(_mmap == MAP_FAILED)
  {
     printf ("Error mmap \n");
     exit(1);
  }
close (fd);
int i;
// for last block:
double blocks = _stat.st_size/size_f, ost, cel;
ost = modf(blocks, &cel);
int lastblock = _stat.st_size - ((int)cel*size_f);
for(i=0;i<(int)cel;i=i+size_f)
	{
		qsort((char*)_mmap+i, size_f, sizeof(char), comp);
	 	msync((char*)_mmap+i, size_f, MS_SYNC); 
	}
		qsort((char*)_mmap+i+lastblock, lastblock, sizeof(char), comp);
	 	msync((char*)_mmap+i+lastblock, lastblock, MS_SYNC);
printf ("File sorted!\n");
munmap (_mmap, _stat.st_size);
return 0;
}
