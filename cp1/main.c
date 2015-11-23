#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <syslog.h>

int p1, p2;
void f_handler(int sig)
{
         // openlog("test",LOG_PID,LOG_USER);
          p1++;
          syslog(LOG_INFO,"SIG1 %d", p1);
         // closelog();
}

void f_handler1(int sig)
{
         // openlog("test",LOG_PID,LOG_USER);
          p2++;
          syslog(LOG_INFO,"SIG2 %d", p2);
         // closelog();
}
int main (int arg, char **argv)
{
/*if (arg !=3)
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
return 0;*/
     pid_t  pid;
     int    status;
     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("ERROR: forking child process failed\n");
        //  WriteLog("[MONITOR] Fork failed (%s)\n", strerror(errno));
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
          setsid();
          printf("Daemon!\n"); 
          struct sigaction act, act1;
          act.sa_handler=f_handler;
          act1.sa_handler=f_handler1;
          sigaction(SIGUSR1, &act,0);
          sigaction(SIGUSR2, &act1,0);
         // WriteLog("[MONITOR] Start Daemon\n");
         // openlog("test",LOG_PID,LOG_USER);
          syslog(LOG_INFO,"start demon");
         // closelog();
          close(STDIN_FILENO);
          close(STDOUT_FILENO);
          close(STDERR_FILENO);
          while(1)
          {
            
          } 
          return status;
          }
     else {                                  /* for the parent:      */
          return 0;
     }
}
