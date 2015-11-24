#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <syslog.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int fd;
int p1, p2;
void f_handler(int sig)
{
          p1++;
          syslog(LOG_INFO,"SIG1 %d", p1);
}

void f_handler1(int sig)
{
          p2++;
          syslog(LOG_INFO,"SIG2 %d", p2);
}

char read_buff[BUFSIZ];
char write_buff[BUFSIZ];
char mshello[BUFSIZ] = "Welcome! Use command `mode`+`00 (11, 10 or 01)` to set mode. Use `STAT` to get statistic\n";
void chat(int fd)
{
    int count;
    write(fd,mshello,sizeof(mshello));
    while((count = read(fd, read_buff, sizeof(read_buff))) > 0) {

	if(!strncmp(read_buff,"mode 11",7)) {
	    syslog(LOG_DAEMON, "SIG 1 ENABLED, SIG 2 ENABLED");
	    signal(SIGUSR1, f_handler);
            signal(SIGUSR2, f_handler1);
	} else if(!strncmp(read_buff,"mode 00",7)) {
	    syslog(LOG_DAEMON, "%s", "SIG 1 DISABLED, SIG 2 DISABLED");
	    signal(SIGUSR1, SIG_IGN);
            signal(SIGUSR2, SIG_IGN);
	} else if(!strncmp(read_buff,"mode 01",7)) {
	    syslog(LOG_DAEMON, "SIG 1 DISABLED, SIG 2 ENABLED");
            signal(SIGUSR1, SIG_IGN);
	    signal(SIGUSR2, f_handler1);
	} else if(!strncmp(read_buff,"mode 10",7)) {
	    syslog(LOG_DAEMON, "SIG 1 ENABLED, SIG 2 DISABLED");
            signal(SIGUSR1, f_handler);
	    signal(SIGUSR2, SIG_IGN);
	} else if(!strncmp(read_buff,"STAT",4)) {
	    snprintf(write_buff, sizeof(write_buff),"STAT: USR1=%d, USR2=%d\n", p1, p2);
	    syslog(LOG_DAEMON, "%s", write_buff);
	} else {
        syslog(LOG_DAEMON, "Wrong command. Use `mode ` + `00 (11, 01, 10)`");
        }
        
    };
}

void daemon_ (void)
{
    int ret;
    int port_num = 8888;
    struct sockaddr_in bind_addr;
    int siz = sizeof(bind_addr);
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(fd < 0){
	syslog(LOG_DAEMON,"Error on create socket (%d)\n", fd);
	exit(fd);
    }
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind_addr.sin_port = htons(port_num);
    ret = bind(fd, (struct sockaddr*)&bind_addr,sizeof(bind_addr));
    if(ret < 0) {
	syslog(LOG_DAEMON,"Error on binding to port %d (%d)\n", port_num, ret);
	exit(ret);
    }
    ret = listen(fd, 1);
    if(ret < 0) {
	syslog(LOG_DAEMON,"Error on listen (%d)\n", ret);
	exit(ret);
    }
    while(1) {
	struct sockaddr_in list_addr;
	int soc;
	soc = accept(fd, (struct sockaddr*)&list_addr,&siz);
	if(soc < 0) {
	    syslog(LOG_DAEMON, "accept connection failed\n");
	    exit(-1);
	}
	chat(soc);
	close(soc);
    }
}

int main (int arg, char **argv)
{
     pid_t  pid, pid1;
     int    status;
     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {          /* for the child process:         */
          setsid();
          pid1 = getpid();
          printf("Daemon! PID = %d Use `telnet localhost 8888 to set mode or get stat`\n",pid1); 
          syslog(LOG_INFO,"start demon");
          close(STDIN_FILENO);
          close(STDOUT_FILENO);
          close(STDERR_FILENO);
          daemon_();
          while(1)
          {
            
          } 
          return status;
          }
     else {                                  /* for the parent:      */
          return 0;
     }
}
