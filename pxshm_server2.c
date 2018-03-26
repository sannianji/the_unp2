#include "cliserv2.h"

int main(int argc,char **argv)
{
	int fd,index,lastnoverflow,temp;
	long offset;
	struct shmstruct *ptr;

	if(argc!=2)
		err_quit("usage: server2 <name>");

	shm_unlink(argv[1]);

	if((fd=shm_open(argv[1],O_RDWR|O_CREAT|O_EXCL,FILE_MODE))<0)
		err_sys("shm_open");
	if((ptr=mmap(NULL,sizeof(struct shmstruct),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
		err_sys("mmap");
	if(ftruncate(fd,sizeof(struct shmstruct))==-1)
		err_sys("ftruncate");
	if(close(fd)<0)
		err_sys("close");
	
	for(index=0;index<NMESG;index++)
		ptr->msgoff[index]=index*MESGSIZE;

	if(sem_init(&ptr->mutex,1,1)==-1)
		err_sys("sem_init");
	if(sem_init(&ptr->nempty,1,NMESG)==-1)
		err_sys("sem_init");
	if(sem_init(&ptr->nstored,1,0)==-1)
		err_sys("sem_init");
	if(sem_init(&ptr->noverflowmutex,1,1)==-1)
		err_sys("sem_init");
	
	index=0;
	lastnoverflow=0;
	for(;;)
	{
		if(sem_wait(&ptr->nstored)==-1)
			err_sys("sem_wait");
		if(sem_wait(&ptr->mutex)==-1)
			err_sys("sem_wait");
		offset=ptr->msgoff[index];
		printf("index=%d:%s\n",index,&ptr->msgdata[offset]);
		if(++index>NMESG)
			index=0;
		if(sem_post(&ptr->mutex)==-1)
			err_sys("sem_post");
		if(sem_post(&ptr->nempty)==-1)
			err_sys("sem_post");
		
		if(sem_wait(&ptr->noverflowmutex)==-1)
			err_sys("sem_wait");
		temp=ptr->noverflow;
		if(sem_post(&ptr->noverflowmutex)==-1)
			err_sys("sem_post");

		if(temp!=lastnoverflow)
		{
			printf("noverflow=%d\n",temp);
			lastnoverflow=temp;
		}
	}
	exit(0);
}
