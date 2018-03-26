#include "unp.h"
#define SEM_NAME "/mysem"

int count=0;

int main(int argc,char **argv)
{
	int i,nloop;
	sem_t *mutex;
	pid_t pid;
	if(argc!=2)
		err_quit("usage: incr1 <#loops>");
	nloop=atoi(argv[1]);

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
			printf("child:%ld\n",count++);
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
			printf("parent:%ld\n",count++);
			if(sem_post(mutex)==-1)
				err_sys("sem_post");
		}
		exit(0);
	}
}
