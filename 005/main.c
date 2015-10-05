#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void f_handler(int sig)
{
printf("\nGoodbye!\n");
exit(0);
}

int main (void)
{
struct sigaction act;
act.sa_handler=f_handler;
sigaction(SIGINT, &act,0);
//pid_t pid;
//pid = fork();
char path[256];
char buf[256];
path[0]=0;
sprintf(path,"%s:%s",getenv("PATH"),getcwd(buf,256));
setenv("PATH",path,1);
printf("******* Welcome to MyShell *******");
while (1)
{
printf("\n ->");
char buf[256];
scanf("%s",buf);
if (buf=="") printf ("\n ->");
system(buf);
}
return 0;
}
