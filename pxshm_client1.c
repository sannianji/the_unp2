#include "unp.h"

struct shmstruct{
	int count;
};

sem_t *mutex;

int main(int argc,char **argv)
{
	int fd,i,nloop;
	pid_t pid;
	struct shmstruct *ptr;
	
	if(argc!=4)
		err_quit("usage: client1 <shmname> <semname> <#loops>");

	nloop=atoi(argv[3]);

	if((fd=shm_open(argv[1],O_RDWR|O_CREAT,FILE_MODE))==-1)
		err_sys("shm_open");

	if((ptr=mmap(NULL,sizeof(struct shmstruct),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
		err_sys("mmap");

	if(close(fd)<0)
		err_sys("close");
	if((mutex=sem_open(argv[2],0))==SEM_FAILED)
		err_sys("sem_open");

	pid=getpid();
	for(i=0;i<nloop;i++)
	{
		if(sem_wait(mutex)==-1)
			err_sys("sem_wait");
		printf("pid %ld:%d\n",(long)pid,ptr->count++);
		if(sem_post(mutex)<0)
			err_sys("sem_post");
	}
	exit(0);
}
