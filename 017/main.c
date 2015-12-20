#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <malloc.h>

#define STACK_SIZE 10000

struct params{
  double low;
  double high;
  double res;
  pid_t thread;
};

 typedef struct params par;

int func(void *arg)
{ 
// low = ((double)N/n)*i;
// high = ((double)N/n)*(i+1);
  par *tmp = (par*)arg;
  double delta = tmp->high - tmp->low;
  double mid = (tmp->high + tmp->low)/2;
  tmp->res = delta * fabs(sin(mid));
  return 0;
}

int main (int arg, char **argv)
{
  double N;
  int n;
  if (arg == 1)
   {
     printf("Please enter arguments:\n");
     printf("Interval = ");
     scanf("%lf",&N);
     printf("Threads count (from 1 to 500) = ");
     scanf("%d",&n);
   }
  else   if (arg == 2)
   {
     N = atof(argv[1]);
     printf("Please enter threads count (from 1 to 500): ");
     scanf("%d",&n);
   }
  else   if (arg > 3)
   {
     printf("Too many args\n");
     exit(1);
   }
  else
   {
     N = atof(argv[1]);
     n = atoi(argv[2]);
   }
  if ((n<=0)||(n>500))
   {
     printf("Invalid threads count value (must be from 1 to 500)\n");
     exit(1);
   }
  if (N<0) N = fabs(N);
  double rezult = 0.0;
  struct params threads[n];
  int i;
  pid_t ifclone;
  char *stack;
  char *stackTop;
  stack = (char*)malloc(STACK_SIZE * n);
  stackTop = stack;
  for (i=0; i<n; i++)
   {
     threads[i].low = ((double)N/n)*i;
     threads[i].high = ((double)N/n)*(i+1);
     stackTop += STACK_SIZE;
     ifclone = clone(func, stackTop, CLONE_VM|SIGCHLD, &threads[i]);
     if (ifclone == -1)
      {
        printf("Create clone error\n");
        perror("clone error");
        exit(1);
      }
     threads[i].thread = ifclone;
    // printf("Thread %d start\n", i);
   }
   int status;
   pid_t wpid;
 for (i=0; i<n; i++)
   {
      wpid = waitpid(threads[i].thread, &status, 0);
      if (wpid == -1)
      {
        printf("Waitpid error\n");
        perror("waitpid error");
        exit(1);
      }
   }
 for (i=0; i<n; i++)
   {
     rezult += threads[i].res;
    // delete [] threads[i].thread;
   }
 //delete threads;
 printf("Result = %f\n", rezult);
return 0;
}
