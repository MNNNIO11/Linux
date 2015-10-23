#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int c, char **v)
{
FILE *fil = popen(v[1], "r");
char *p;
//char buf[];
char read_f[100];
while (fgets(read_f,sizeof(read_f),fil))
{
int N=strlen(read_f);
//char buf[N];
int i;
for (i=0;i<N/2;i++)
{
char p;
p=read_f[i];
read_f[i]=read_f[N-i-1];
read_f[N-i-1]=p;
}
printf("%s\n",read_f);
}
pclose (fil);
return 0;
}
