#include "unp.h"
#define NBUFF 10

int nitems;
struct {
	int buff[NBUFF];
	sem_t mutex,nempty,nstored;
}shared;

void *produce(void *),*consume(void*);

int main(int argc,char **argv)
{
	pthread_t tid_produce,tid_consume;
	int err;
	if(argc!=2)
		err_quit("usage: prodcons2 <#items>");
	nitems=atoi(argv[1]);

	if(sem_init(&shared.mutex,0,1)==-1)
		err_sys("shared.mutex sem_init");
	if(sem_init(&shared.nempty,0,NBUFF)<0)
		err_sys("shared.nempty sem_init");
	if(sem_init(&shared.nstored,0,0)==-1)
		err_sys("shared.nstored sem_init");
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
		err_sys("tid_produce pthread_join");
	}
	if((err=pthread_join(tid_consume,NULL))!=0)
	{
		errno=err;
		err_sys("tid_consume pthread_join");
	}
	if(sem_destroy(&shared.mutex)==-1)
		err_sys("shared.mutex sem_destroy");
	if(sem_destroy(&shared.nempty)==-1)
		err_sys("shared.nempty sem_destroy");
	if(sem_destroy(&shared.nstored)==-1)
		err_sys("shared.nstored sem_destroy");

	exit(0);
}

void *produce(void *arg)
{
	int i;
	for(i=0;i<NBUFF;i++)
	{
		if(sem_wait(&shared.nempty)==-1)
			err_sys("produce sem_wait shared.nempty");
		if(sem_wait(&shared.mutex)==-1)
			err_sys("produce sem_wait shared.mutex");
		shared.buff[i%NBUFF]=i;
		if(sem_post(&shared.mutex)==-1)
			err_sys("produce sem_wait shared.mutex");
		if(sem_post(&shared.nstored)==-1)
			err_sys("produce sem_wait shared.nstored");
	}
	return NULL;
}
void *consume(void *arg)
{
	int i;
	for(i=0;i<NBUFF;i++)
	{
		if(sem_wait(&shared.nstored)==-1)
			err_sys("consume sem_wait shared.nstored");
		if(sem_wait(&shared.mutex)==-1)
			err_sys("consume sem_wait shared.mutex");
		if(shared.buff[i%NBUFF]!=i)
			printf("buff[%d]=%d\n",i,shared.buff[i%NBUFF]);
		if(sem_post(&shared.mutex)==-1)
			err_sys("consume sem_post shared.mutex");
		if(sem_post(&shared.nempty)==-1)
			err_sys("consume sem_post shared.nempty");
	}
	return (NULL);
}
