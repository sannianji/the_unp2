#include "unp.h"

void server(int,int);
static void err_sys(const char *str1,const char *str2)
{
	printf(str1,str2);
	fflush(stdout);
	exit(0);
}
int main(int argc,char **argv)
{
	int readfifo,writefifo,dummyfd,fd;
	char *ptr,buff[MAXLINE+1],fifoname[MAXLINE];
	pid_t pid;
	ssize_t n;
	
	if((mkfifo(SERV_FIFO,FILE_MODE)<0)&&(errno!=EEXIST))
		err_sys("can't create %s",SERV_FIFO);
}
