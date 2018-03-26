#include "cliserv2.h"
extern void Sleep_us(int);
int main(int argc,char **argv)
{
	int fd,i,nloop,nusec;
	pid_t pid;
	char mesg[MESGSIZE];
	long offset;
	struct shmstruct *ptr;
		
	if(argc!=4)
		err_quit("usage: client2 <name><#loops><#usec>");

	nloop=atoi(argv[2]);
	nusec=atoi(argv[3]);
	
	if((fd=shm_open(argv[1],O_RDWR,FILE_MODE))==-1)
		err_sys("shm_open");
	if((ptr=mmap(NULL,sizeof(struct shmstruct),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
		err_sys("mapp");
	if(close(fd)<0)
		err_sys("close");
	pid=getpid();
	for(i=0;i<nloop;i++)
	{
		Sleep_us(nusec);
		snprintf(mesg,MESGSIZE,"pid %ld:message %d",(long)pid,i);
		if(sem_trywait(&ptr->nempty)==-1)
		{
			if(errno==EAGAIN)
			{
				if(sem_wait(&ptr->noverflowmutex)==-1)
					err_sys("sem_wait");
				ptr->noverflow++;
				if(sem_post(&ptr->noverflowmutex)==-1)
					err_sys("sem_post");
				continue;
			}
			else
				err_sys("sem_trywait error");
		}
		if(sem_wait(&ptr->mutex)==-1)
			err_sys("sem_wait");
		offset=ptr->msgoff[ptr->nput];
		if(++(ptr->nput)>=NMESG)
			ptr->nput=0;
		if(sem_post(&ptr->mutex)==-1)
			err_sys("sem_post");
		strcpy(&ptr->msgdata[offset],mesg);
		if(sem_post(&ptr->nstored)==-1)
			err_sys("sem_post");
	}
	exit(0);
}
