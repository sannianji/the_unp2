#include "unp.h"

#define NBUFF 10
#define MAXNTHREADS 100

struct {
	int buff[NBUFF];
	int nput;
	int nputval;
	sem_t mutex,nempty,nstored;
}shared;

void *produce(void*),*consume(void *);
int nitems,nproducers;
int main(int argc,char **argv)
{
	int i;
	pthread_t tid_produce[MAXNTHREADS],tid_consume;
	int err;
	if(argc!=3)
		err_quit("usage: prodcuns3_pxsem <#items><#nproducers>\n");

	nitems=atoi(argv[1]);
	nproducers=min(atoi(argv[2]),MAXNTHREADS);
	
	if(sem_init(&shared.mutex,0,1)==-1)
		err_sys("sem_init shared.mutex");
	if(sem_init(&shared.nempty,0,NBUFF)==-1)
		err_sys("sem_init shared.nempty");
	if(sem_init(&shared.nstored,0,0)==-1)
		err_sys("sem_init shared.nstored");
	
	for(i=0;i<nproducers;i++)
	{
		count[i]=0;	
		if((err=pthread_create(&tid_produce[i],NULL,produce,&count[i]))!=0)
		{
			errno=err;
			err_sys("produce pthread_create");
		}
	}
	if((err=pthread_create(&tid_consume,NULL,consume,NULL))!=0)
	{
		errno=err;
		err_sys("consume pthread_create");
	}
	for(i=0;i<nproducers;i++)
	{
		if((err=pthread_detach(tid_produce[i]))!=0)
		{
			errno=err;
			err_sys("produce pthread_join");
		}
//		printf("count[%d]=%d\n",i,count[i]);
	}
	if((err=pthread_join(tid_consume,NULL))!=0)
	{
		errno=err;
		err_sys("consume pthread_join");
	}
	
	if(sem_destroy(&shared.mutex)==-1)
		err_sys("sem_destroy shared_mutex");
	if(sem_destroy(&shared.nstored)==-1)
		err_sys("sem_destroy shared_nstored");
	if(sem_destroy(&shared.nempty)==-1)
		err_sys("sem_destroy shared_nempty");

	exit(0);
}


void* produce(void* arg)
{
	for(;;)
	{
		if(sem_wait(&shared.nempty)==-1)
			err_sys("sem_wait shared_nempty");
		if(sem_wait(&shared.mutex))
			err_sys("sem_wait shared_mutex");

		if(shared.nput>=nitems)
		{
			if(sem_post(&shared.nempty)==-1)
				err_sys("produce sem_post shared_nempty");
			if(sem_post(&shared.mutex)==-1)
				err_sys("produce sem_post shared_mutex");
			return NULL;
		}
		shared.buff[shared.nput%NBUFF]=shared.nputval;
		shared.nput++;
		shared.nputval++;

		if(sem_post(&shared.mutex)==-1)
			err_sys("produce sem_post shared_mutex");
		if(sem_post(&shared.nstored)==-1)
			err_sys("produce sem_post shared_nstored");
		*((int *)arg)+=1;
	}
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

