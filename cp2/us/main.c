#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

char buf [64];
char rwbuf [BUFSIZ];
char irqbuf [BUFSIZ];
int fd;
unsigned int irq_count;

struct dev_stat {
int f_open;
int f_read;
int f_write;
int f_ioctl;
int f_irq;
};

struct mynet_device
{
  struct net_device *dev;
  /**/
  int xmit;
  int bytes;
  unsigned char netdata[256];
} mynetdev;

struct pinfo {
int pid;
char buf[BUFSIZ];
};

struct dev_stat info;
struct pinfo p_info;

int main (int arg, char * argv[]) {

uint32_t count;	
if (arg != 2)
{
  printf ("Enter argument:\n");
  printf ("wr - write to device\n");
  printf ("rd - read from device\n");
  printf ("io - show wr/rd device statistic\n");
  printf ("pinfo - show current process info\n");
  printf ("irq - show IRQ (for device '19') count\n");
  exit (1);
}  
strcpy (buf, argv[1]);
fd = open ("./../mydev", O_RDWR);
if(-1 == fd)
{
printf("Error open devise\n");
exit (1);
}

if (0 == strcmp (buf, "wr"))
{
  if (write (fd,"test",sizeof("test")) == -1)
    {
       printf ("Error write\n");
       exit (1);
    }
  else printf ("Write success\n");
  
}

if (0 == strcmp(buf, "rd"))
{
  if (read (fd,rwbuf,sizeof(rwbuf)) == -1)
    {
       printf ("Error read\n");
       exit (1);
    }
  else printf ("Read success\n");
}

if (0 == strcmp(buf, "io"))
{ 
ioctl(fd, 0x123, &info);
        printf(" STATISTIC:\n");
	printf(" opened: %d\n", info.f_open);
	printf(" readed: %d\n", info.f_read);
	printf(" writen: %d\n", info.f_write);
	printf(" ioctl:  %d\n", info.f_ioctl);
       // printf("IRQ:  %d\n", info.f_irq);

}

if (0 == strcmp(buf, "irq"))
{ 
ioctl(fd, 0x134, &irq_count);
        printf(" IRQ:%d\n",irq_count);
}

if (0 == strcmp(buf, "pinfo"))
{ 
ioctl(fd, 0x145, &p_info);
        printf("%s\n", p_info.buf);
       // printf(" PID = %d\n", p_info.pid);
       // printf(" Info: %s\n", p_info.buf);
}
if (0 == strcmp(buf, "net"))
{ 
ioctl(fd, 0x156, &mynetdev);
        printf("Packets: %d\n", mynetdev.xmit);
        printf("Bytes: %d\n", mynetdev.bytes);
	printf("Bytes value: %s\n", mynetdev.netdata);
}
close (fd);

return 0;
}
