#include <stdio.h>
#include <string.h>
int main (int argc, char **argv, char **envp)
{
if (!envp)
{
return -1;
} 
while (*envp)
  {
    char *p=strstr(*envp,"PWD=");
    if (p==*envp)
      {
        printf("Current path = %s\n",(p+4));
      }
    envp++;
  }
  return 0;
}
