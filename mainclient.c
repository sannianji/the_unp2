#include "unp.h"
static void err_sys(const char *str1,const char *str2)
{
	printf(str1,str2);
	fflush(stdout);
	exit(-1);
}
int main(int argc,char **argv)
{
	int readfifo,writefifo;
	size_t len;
	ssize_t n;
	char *ptr,fifoname[MAXLINE],buff[MAXLINE];
	pid_t pid;

	pid=getpid();
	snprintf(fifoname,sizeof(fifoname),"/tmp/fifo.%ld",(long)pid);
	if((mkfifo(fifoname,FILE_MODE)<0)&&(errno!=EEXIST))
		err_sys("can't create %s",fifoname);
	
	snprintf(buff,sizeof(buff),"%ld ",(long) pid);
	len=strlen(buff);
	ptr=buff+len;

	fgets(ptr,MAXLINE-len,stdin);

	len=strlen(buff);
	writefifo=open(SERV_FIFO,O_WRONLY,0);
	if(write(writefifo,buff,len)!=len)
		err_sys("write fifo error:%s","write to server_fifo error");
	printf("client:the fifoname is %s",fifoname);
	fflush(stdout);
	if((readfifo=open(fifoname,O_RDONLY,0))<0)
		err_sys("open fifo error:%s",strerror(errno));

	while((n=read(readfifo,buff,MAXLINE))>0)
		if(write(STDOUT_FILENO,buff,n)!=n)
			err_sys("write error:%s",strerror(errno));

	close(readfifo);
	unlink(fifoname);
	exit(0);
}
