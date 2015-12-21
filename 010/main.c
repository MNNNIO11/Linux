#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc,  char **v)
{
  FILE *file_wr;
  char buf[BUFSIZ];
  int count = 0;
  if (argc != 3)
  {
    fprintf(stderr, "Usage: proc1|./tie string filename|proc2|...\n");
    exit(EXIT_FAILURE);
  }
  file_wr = fopen(v[2],"w");
  if (!file_wr)
  {
    fprintf(stderr, "Can't open file %s", v[2]);
    exit(EXIT_FAILURE);
  }
  while (fgets(buf, sizeof(buf), stdin))
  {
    fputs(buf, stdout);
    if (strstr(buf, v[1]))
    count++;
  }
  if (count)
    fprintf(file_wr, "String \"%s\" found %d times", v[1], count);
  else
    fprintf(file_wr, "String \"%s\" not found", v[1]);
  fclose(file_wr);
  return 0;

}
