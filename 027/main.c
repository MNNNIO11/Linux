#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)


static int              /* Start function for cloned child */
childFunc(const char *arg)
{
    struct utsname uts;

    /* Change hostname in UTS namespace of child */

    if (sethostname(arg, strlen(arg)) == -1)
        errExit("sethostname");

    /* Retrieve and display hostname */

    if (uname(&uts) == -1)
        errExit("uname");
    printf("uts.nodename in child:  %s\n", uts.nodename);
	
    sleep(100);

    return 0;           /* Terminates child */
}

#define STACK_SIZE (1024 * 1024)    /* Stack size for cloned child */

static char child_stack[STACK_SIZE];

void isp(char *arg)
{
     pid_t  pid;
     int    status;
     struct utsname uts;
     
     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
          {     /* execute the command  */
               unshare(CLONE_NEWUTS);
               if (sethostname(arg, strlen(arg)) == -1)
               errExit("sethostname");

               if (uname(&uts) == -1)
               errExit("uname");
               printf("uts.nodename in child:  %s\n", uts.nodename);
               sleep(100);

          }
     }
     else {  
    
    if (uname(&uts) == -1)
        errExit("uname");
        printf("uts.nodename in parent: %s\n",uts.nodename);                                

     }
}

int
main(int argc, char *argv[])
{
   // pid_t child_pid;
    struct utsname uts;
		char cmd[BUFSIZ];

    if (argc < 2) {
        fprintf(stderr, "Usage: sudo %s <child-hostname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    isp(argv[1]); 

    exit(EXIT_SUCCESS);
}
