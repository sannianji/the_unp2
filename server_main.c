#include "unp.h"

void server(int,int);
static  void  err_sys(const char *str1,const char *str2)
{
	printf("%s %s",str1,str2);
	fflush(stdout);
	exit(-1);
}
int main(int argc,char **argv)
{
	int readfd,writefd;
	
	if((mkfifo(FIFO1,FILE_MODE)<0)&&(errno!=EEXIST))
		err_sys("can't create %s",FIFO1);
	if((mkfifo(FIFO2,FILE_MODE)<0)&&(errno!=EEXIST))
	{
		unlink(FIFO1);
		err_sys("can't create %s",FIFO1);
	}
	
	readfd=open(FIFO1,O_RDONLY,0);
	writefd=open(FIFO2,O_WRONLY,0);
	
	server(readfd,writefd);
	exit(0);
}
