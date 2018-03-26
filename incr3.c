#include "unp.h"

struct shared{
	sem_t mutex;
	int count;
}shared;

int main(int argc,char **argv)
{
	int fd,i,nloop;
	struct shared *ptr;
	pid_t pid;
	if(argc!=3)
		err_quit("usage : incr3 <pathname> <nloop>");
	nloop=atoi(argv[2]);

	if((fd=open(argv[1],O_RDWR|O_CREAT,FILE_MODE))<0)
		err_sys("open");
	if(write(fd,&shared,sizeof(shared))!=sizeof(shared))
		err_sys("write");
	if((ptr=mmap(NULL,sizeof(struct shared),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
		err_sys("mmap");

	if(close(fd)<0)
		err_sys("close");

	if(sem_init(&ptr->mutex,1,1)==-1)
		err_sys("sem_init");
	
	setbuf(stdout,NULL);

	if((pid=fork())<0)
		err_sys("fork");
	else if(pid==0)
	{
		for(i=0;i<nloop;i++)
		{
			if(sem_wait(&ptr->mutex)==-1)
				err_sys("sem_wait");
			printf("child:%d\n",ptr->count++);
			if(sem_post(&ptr->mutex)==-1)
				err_sys("sem_post");
		}
	}
	else if(pid>0)
	{
		for(i=0;i<nloop;i++)
		{
			if(sem_wait(&ptr->mutex)==-1)
				err_sys("sem_wait");
			printf("child:%d\n",ptr->count++);
			if(sem_post(&ptr->mutex)==-1)
				err_sys("sem_post");
		}
	}
	exit(0);
		
		
}
