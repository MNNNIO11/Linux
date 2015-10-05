#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

int ppid (int pid)
{ 
  char buf[20];
  char name[100];
  char read_f[100];
  int ppidret=0;
  FILE *file_pid;
  sprintf(name, "/proc/%d/status", pid);
 // if (access(name)!=0)
 // {
 //     printf("Error: pid %d not found\n",pid);
 //     exit(1);
 // }  
  file_pid=fopen(name,"r");
  if (!file_pid)
   {
      printf("Error: can't open file %s\n",name);
      exit(1);
   }
  while (fgets(read_f,sizeof(read_f),file_pid))
   {
     if (strstr(read_f,"PPid"))
      {
         strcpy(buf,&read_f[6]);
         ppidret = atoi(buf);
         break;
    }
   } 
  fclose(file_pid);
  return ppidret;
}



int main (int c, char **v)
{
   if (c == 1)
    {
      DIR *dir;
      struct dirent *entry;
      dir = opendir("/proc/");
      if (!dir)
      {
        printf("Error: can't open /proc/");
        exit(1);
      }
      while ((entry=readdir(dir)) != NULL)
      {
         if (entry->d_type == 4)
           {
               int pid = atoi(entry->d_name);
               if (pid != 0){
               while (pid != 0)
               {
    		printf("%d->", pid);
                pid=ppid(pid);  
               }
              printf("()\n");
              }
            } 
      }
      closedir(dir);
    } 
   if (c == 2)
   { 
   int pid = atoi(v[1]);
   if (pid !=0){   
while (pid != 0)
  {
    printf("%d->", pid);
    pid=ppid(pid);  
  }
  printf("()\n");
  }	
  }
  return 0;
}
