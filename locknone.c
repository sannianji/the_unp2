#include "unp.h"

#define SEQFILE "seqno"

void my_lock(int),my_unlock(int);
int main(int argc,char  **argv)
{
	int fd;
	long i,seqno;
	pid_t pid;
	ssize_t  n;
	char line[MAXLINE+1];

	pid=getpid();
	if((fd=open(SEQFILE,O_RDWR,FILE_MODE))<0)
		err_sys("open");
	for(i=0;i<20;++i)
	{
		my_lock(fd);
	
		if(lseek(fd,0L,SEEK_SET)<0)
			err_sys("lseek");

		if((n=read(fd,line,MAXLINE))<0)
			err_sys("read");
		line[n]='\0';

		n=sscanf(line,"%ld\n",&seqno);

		printf("%s: pid=%ld,seq#=%ld\n",argv[0],(long)pid,seqno);

		seqno++;
		snprintf(line,sizeof(line),"%ld\n",seqno);

		if(lseek(fd,0L,SEEK_SET)<0)
			err_sys("lseek");
		if(write(fd,line,strlen(line))!=strlen(line))
			err_sys("write");

		my_unlock(fd);
	}
	exit(0);
}
void my_lock(int arg)
{
return;
}

void my_unlock(int arg)
{
return;
}
