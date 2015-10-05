#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main (int arg, char **argv)
{
if (arg !=3)
{
printf("Error\n");
exit(1);
}
int pid=atoi(argv[1]);
int sig=atoi(argv[2]);
if (sig == 15)
{
if(kill(pid,sig)==-1)
{
printf("Error\n");
exit(1);
}
else
{
printf("Process %d was killed\n",pid);
}
}
else 
{
printf("Incorrect signal\n");
return 1;
}
return 0;
}
