#include "unp.h"
ssize_t readline(int,void*,ssize_t);
void server(int,int);
static void err_sys(const char *str1,const char *str2)
{
	printf(str1,str2);
	fflush(stdout);
	exit(0);
}
static void err_msg(const char *str1,const char *str2)
{
	printf(str1,str2);
	fflush(stdout);
}
int main(int argc,char **argv)
{
	int readfifo,writefifo,dummyfd,fd;
	char *ptr,buff[MAXLINE+1],fifoname[MAXLINE];
	pid_t pid;
	ssize_t n;
	
	if((mkfifo(SERV_FIFO,FILE_MODE)<0)&&(errno!=EEXIST))
		err_sys("can't create %s",SERV_FIFO);
	
	readfifo=open(SERV_FIFO,O_RDONLY,0);
	dummyfd=open(SERV_FIFO,O_WRONLY,0);//never used
	
	while((n=readline(readfifo,buff,MAXLINE))>0)
	{
		if(buff[n-1]=='\n')
			n--;
		buff[n]='\0';
		
		if((ptr=strchr(buff,' '))==NULL)
		{
			err_msg("bogus request: %s\n",buff);
			continue;
		}
		*ptr++=0;
		pid=atol(buff);
		snprintf(fifoname,sizeof(fifoname),"/tmp/fifo.%ld",(long)pid);
		printf("server:the fifoname is %s\n",fifoname);
		fflush(stdout);
		if((writefifo=open(fifoname,O_WRONLY,0))<0)
		{
			err_msg("can't open: %s\n",fifoname);
			continue;
		}
		if((fd=open(ptr,O_RDONLY))<0)
		{
			snprintf(buff+n,sizeof(buff)-n,": can't open,%s\n",strerror(errno));
			n=strlen(ptr);
			write(writefifo,ptr,n);
			close(writefifo);
		}
		else
		{
			while((n=read(fd,buff,MAXLINE))>0)
				write(writefifo,buff,n);
			close(fd);
			close(writefifo);
		}
	}
	printf("\nn=%d\nthe fifoname is:%s",n,fifoname);
	exit(0);
}
