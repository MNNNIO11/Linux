#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main (int arg, char **argv)
{
  int pid;
  char dir_name[BUFSIZ];
  DIR *dir;
  struct dirent *entry;
  
  if (arg == 1)
   {
     printf("Please enter argument:\n");
     printf("PID = ");
     scanf("%d",&pid);
   }
    else if (arg == 2)
   {
     pid = atof(argv[1]);
   }
    else
   {
     printf("Need 1 argument\n");
     exit(1);
   }
   
   sprintf(dir_name, "/proc/%d/task",pid);
   dir = opendir(dir_name);
   if (!dir)
   {
     printf("Can't open dir\n");
     perror ("error open dir");
     exit(1); 
   }
   while ((entry = readdir(dir)) != NULL)
   {
     if (atoi(entry->d_name))
     printf("-->%s\n",entry->d_name);
   }
   closedir(dir);
return 0;
}
