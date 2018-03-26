#include "unp.h"

#define SEM_NAME "/mysem"

int main(int argc,char **argv)
{
	int fd,i,nloop,zero=0;
	int *ptr;
	sem_t *mutex;
	pid_t pid;

	if(argc!=3)
		err_quit("usage: incr2 <pathname> <#loops>");
	nloop=atoi(argv[2]);

	if((fd=open(argv[1],O_RDWR|O_CREAT,FILE_MODE))<0)
		err_sys("open");
	if(write(fd,&zero,sizeof(int))!=sizeof(int))
		err_sys("write");
	if((ptr=mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
		err_sys("mmap");
	
	if(close(fd)<0)
		err_sys("close");
	if((mutex=sem_open(SEM_NAME,O_CREAT|O_EXCL,FILE_MODE,1))==SEM_FAILED)
		err_sys("sem_open");
	if(sem_unlink(SEM_NAME)==-1)
		err_sys("sem_unlink");

	setbuf(stdout,NULL);
	if((pid=fork())<0)
		err_sys("fork");
	else if(pid==0)
	{
		for(i=0;i<nloop;i++)
		{
			if(sem_wait(mutex)==-1)
				err_sys("sem_wait");
			printf("child:%d\n",(*ptr)++);
			if(sem_post(mutex)==-1)
				err_sys("sem_post");
		}
		exit(0);
	}
	else if(pid>0)
	{
		for(i=0;i<nloop;i++)
		{
			if(sem_wait(mutex)==-1)
				err_sys("sem_wait");
			printf("parent:%d\n",(*ptr)++);
			if(sem_post(mutex)==-1)
				err_sys("sem_post");
		}
		exit(0);
	}
}
