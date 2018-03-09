#include "unp.h"
void client(int,int),server(int,int);
static void err_sys(const char * str1,const char *str2)
{
	printf("%s %s\n",str1,str2);
	fflush(stdout);
	exit(-1);

}
int main(int argc,char **argv)
{
	int readfd,writefd;
	pid_t childpid;
	

	if((mkfifo(FIFO1,FILE_MODE)<0)&&(errno!=EEXIST))
		err_sys("can't create %s",FIFO1);
	if((mkfifo(FIFO2,FILE_MODE)<0)&&(errno!=EEXIST))
	{
		unlink(FIFO1);
		err_sys("can't create %s",FIFO2);
	}
	if((childpid=fork())==0)//child
	{
		readfd=open(FIFO1,O_RDONLY,0);
		writefd=open(FIFO2,O_WRONLY,0);
		server(readfd,writefd);
		exit(0);
	}
	//parent
	
	writefd=open(FIFO1,O_WRONLY,0);
	readfd=open(FIFO2,O_RDONLY,0);
	
	client(readfd,writefd);
	waitpid(childpid,NULL,0);
	
	close(readfd);
	close(writefd);
	
	unlink(FIFO1);
	unlink(FIFO2);
	exit(0);

}
