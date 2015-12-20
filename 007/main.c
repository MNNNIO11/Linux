#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <malloc.h>

struct params{
  double low;
  double high;
  double res;
  pthread_t thread;
};

 typedef struct params par;

void *func(void *arg)
{ 
// low = ((double)N/n)*i;
// high = ((double)N/n)*(i+1);
  par *tmp = (par*)arg;
  double delta = tmp->high - tmp->low;
  double mid = (tmp->high + tmp->low)/2;
  tmp->res = delta * fabs(sin(mid));
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
  for (i=0; i<n; i++)
   {
     threads[i].low = ((double)N/n)*i;
     threads[i].high = ((double)N/n)*(i+1);

     if (pthread_create(&threads[i].thread, NULL, func, &threads[i]) != 0)
      {
        printf("Create thread error\n");
        perror("can't create thread");
        exit(1);
      }
    // printf("Thread %d start\n", i);
   }
 for (i=0; i<n; i++)
   {
     if(pthread_join( threads[i].thread, NULL) != 0)
       {
        printf("Join thread error\n");
        perror("can't join thread");
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
