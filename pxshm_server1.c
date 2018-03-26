#include "unp.h"

struct shmstruct{
	int count;
};

sem_t *mutex;

int main(int argc,char **argv)
{
	int fd;
	struct shmstruct *ptr;
	if(argc!=3)
		err_quit("usage: server1 <shmname><semname>");

	shm_unlink(argv[1]);

	if((fd=shm_open(argv[1],O_RDWR|O_CREAT|O_EXCL,FILE_MODE))==-1)
		err_sys("shm_open");

	if(ftruncate(fd,sizeof(struct shmstruct))<0)
		err_sys("ftruncate");

	if((ptr=mmap(NULL,sizeof(struct shmstruct),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
		err_sys("mmap");

	if(close(fd)<0)
		err_sys("close");

	sem_unlink(argv[2]);
	if((mutex=sem_open(argv[2],O_CREAT|O_EXCL,FILE_MODE,1))==SEM_FAILED)
		err_sys("sem_open");

	if(sem_close(mutex)==-1)
		err_sys("sem_close");
	
	exit(0);
}

