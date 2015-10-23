#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

void f_handler(int sig)
{
printf("\nGoodbye!\n");
exit(0);
}

void isp(char *arg[])
{
     pid_t  pid;
     int    status;
     
     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
          if (execvp(arg[0], arg) < 0) {     /* execute the command  */
               printf("ERROR: exec failed\n");
               exit(1);
          }
     }
     else {                                  /* for the parent:      */
          while (wait(&status) != pid)       /* wait for completion  */
               ;
     }
}

void obrstr(char str[], char *arg[])
{
   int countwords=0;
   int sc;
   for (sc=0;sc<strlen(str);sc++)
   {
     if (strchr(" ",str[sc])) countwords++; 
   }
   char *p;
   int par=0;
   p=strtok(str," ");
   while (p)
   {
     arg[par]=p;
     p = strtok(0," ");
     par++;
   }
}


int main (void)
{
  struct sigaction act;
  act.sa_handler=f_handler;
  sigaction(SIGINT, &act,0);
  
  char path[256];
  char buf[256];
  path[0]=0;
  sprintf(path,"%s:%s",getenv("PATH"),getcwd(buf,256));
  setenv("PATH",path,1);
 
  char bufp[BUFSIZ];
  char *arg[BUFSIZ];
  printf("******* Welcome to MyShell *******\n");
  while (1)
  {
   printf("->");
   //  scanf("%[^\r\n]",bufp);
   fgets(bufp,sizeof(bufp),stdin);
   //gets(bufp);
   printf("\n");
  // obrstr(bufp,arg);
  // isp(arg);
    system(bufp);
  }
return 0;
}
