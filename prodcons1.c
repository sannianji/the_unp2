#include "unp.h"

#define NBUF 10
#define SEM_MUTEX "mutex"
#define SEM_NEMPTY "nempty"
#define SEM_NSTORED "nstored"

int nitems;
struct{
	int buff[NBUF];
	sem_t *mutex,*nempty,*nstored;
}shared;


void *produce(void *),*consume(void *);
sem_t *Sem_open(const char *path,int flags,int file_mode,int n)
{
	sem_t* sem;
	if((sem=sem_open(path,flags,file_mode,n))==SEM_FAILED)
		err_sys("sem_open");
	return sem;
}
int main(int argc,char **argv)
{
	pthread_t tid_produce,tid_consume;
	int err;
	if(argc!=2)
		err_quit("usage: prodcons1 <#items>\n");

	nitems=atoi(argv[1]);

	shared.mutex=Sem_open(SEM_MUTEX,O_CREAT|O_EXCL,FILE_MODE,1);
	shared.nempty=Sem_open(SEM_NEMPTY,O_CREAT|O_EXCL,FILE_MODE,NBUF);
	shared.nstored=Sem_open(SEM_NSTORED,O_CREAT|O_EXCL,FILE_MODE,0);
	
	if((err=pthread_create(&tid_produce,NULL,produce,NULL))!=0)
	{
		errno=err;
		err_sys("produce pthread_create");
	}
	if((err=pthread_create(&tid_consume,NULL,consume,NULL))!=0)
	{
		errno=err;
		err_sys("consume pthread_create");
	}

	if((err=pthread_join(tid_produce,NULL))!=0)
	{
		errno=err;
		err_sys("produce pthread_join");
	}
	if((err=pthread_join(tid_consume,NULL))!=0)
	{
		errno=err;
		err_sys("consume pthread_join");
	}

	if(sem_unlink(SEM_MUTEX)<0)
		err_sys("sem_unlink",SEM_MUTEX);
	if(sem_unlink(SEM_NEMPTY)<0)
		err_sys("sem_unlink",SEM_NEMPTY);
	if(sem_unlink(SEM_NSTORED)<0)
		err_sys("sem_unlink",SEM_NSTORED);
	exit(0);
	
}
void Sem_wait(sem_t* sem)
{
	int err;
	if((err=sem_wait(sem))<0)
	{
		errno=err;
		err_sys("sem_wait");
	}
	return;
}
void Sem_post(sem_t* sem)
{
	int err;
	if((err=sem_post(sem))<0)
	{
		errno=err;
		err_sys("sem_post");
	}
	return;
}
void* produce(void *arg)
{
	int i;
	for(i=0;i<nitems;i++)
	{
		Sem_wait(shared.nempty);
		Sem_wait(shared.mutex);
		shared.buff[i%NBUF]=i;
		Sem_post(shared.mutex);
		Sem_post(shared.nstored);	
	}
	return NULL;
}
void *consume(void *arg)
{
	int i;
	for(i=0;i<nitems;i++)
	{
		Sem_wait(shared.nstored);
		Sem_wait(shared.mutex);
		if(shared.buff[i%NBUF]!=i)
			printf("buff[%d]=%d\n",i,shared.buff[i%NBUF]);
		Sem_post(shared.mutex);
		Sem_post(shared.nempty);
	}
	return NULL;
}
