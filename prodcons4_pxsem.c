#include "unp.h"
#define NBUFF 10
#define MAXNTHREADS 100

int nitems,nproducers,nconsumers;

struct{
	int buff[NBUFF];
	int nput;
	int nputval;
	int nget;
	int ngetval;
	sem_t mutex,nempty,nstored;
}shared;

void *produce(void *),*consume(void*);

int main(int argc,char **argv)
{
	int i,prodcount[MAXNTHREADS],consumers[MAXNTHREADS];
	pthread_t tid_producer[MAXNTHREADS],tid_consumer[MAXNTHREADS];
	int err;

	if(argc!=4)
		err_quit("usage: prodcons4 <#nitems><#producers><#consumers>\n");

	nitems=atoi(argv[1]);
	nproducers=min(atoi(argv[2]),MAXNTHREADS);
	nconsumers=min(atoi(argv[3]),MAXNTHREADS);

	if(sem_init(&shared.mutex,0,1)==-1)
		err_sys("shared_mutex sem_init");
	if(sem_init(&shared.nempty,0,NBUFF)==-1)
		err_sys("shared_nempty sem_init");
	if(sem_init(&shared.nstored,0,0)==-1)
		err_sys("shared_nstored sem_init");
	
	for(i=0;i<nproducers;i++)
	{
		prodcount[i]=0;
		if((err=pthread_create(&tid_producer[i],NULL,produce,&prodcount[i]))!=0)
		{
			errno=err;
			err_sys("produce pthread_create");
		}
	}

	for(i=0;i<nconsumers;i++)
	{
		consumers[i]=0;
		if((err=pthread_create(&tid_consumer[i],NULL,consume,&consumers[i]))!=0)
		{
			errno=err;
			err_sys("consume pthread_create");
		}
	}
	for(i=0;i<nproducers;i++)
	{
		if((err=pthread_join(tid_producer[i],NULL))!=0)
		{
			errno=err;
			err_sys("produce pthread_join");
		}
		printf("producecr count[%d]=%d\n",i,prodcount[i]);
	}
	for(i=0;i<nconsumers;i++)
	{
		if((err=pthread_join(tid_consumer[i],NULL))!=0)
		{
			errno=err;
			err_sys("consume pthread_join");
		}
		printf("consume count[%d]=%d",i,consumers[i]);
	}
	if(sem_destroy(&shared.nstored)==-1)
		err_sys("shared.nstored sem_destroy");
	if(sem_destroy(&shared.mutex)==-1)
		err_sys("shared.mutex sem_destroy");
	if(sem_destroy(&shared.nempty)==-1)
		err_sys("shared.nempty sem_destroy");
	exit(0);
}
void *produce(void *arg)
{
	for(;;)
	{
		if(sem_wait(&shared.nempty)==-1)
			err_sys("shared.nempty sem_wait");
		if(sem_wait(&shared.mutex)==-1)
			err_sys("shared.mutex sem_wait");
		if(shared.nput>=nitems)
		{
			if(sem_post(&shared.nstored)==-1)
				err_sys("shared.nsotred sem_post");
			if(sem_post(&shared.nempty)==-1)
				err_sys("shared.nempty sem_post");
			if(sem_post(&shared.mutex)==-1)
				err_sys("sjared.mutex sem_post");
			return NULL;
		}
		shared.buff[shared.nput%NBUFF]=shared.nputval;
		shared.nput++;
		shared.nputval++;
		if(sem_post(&shared.mutex)==-1)
			err_sys("shared.mutex post");
		if(sem_post(&shared.nstored)==-1)
			err_sys("shared.nstored post");

		*((int *)arg)+=1;
	}
}
void* consume(void *arg)
{
	int i;
	for(;;)
	{
		if(sem_wait(&shared.nstored)==-1)
			err_sys("shared.nstored sem_wait");
		if(sem_wait(&shared.mutex)==-1)
			err_sys("shared.mutex sem_wait");
		if(shared.ngetval>=nitems)
		{
			if(sem_post(&shared.mutex)==-1)
				err_sys("shared.mutex sem_post");
			if(sem_post(&shared.nstored)==-1)
				err_sys("shared.nstored sem_post");
			return NULL;
		}

		i=shared.nget%NBUFF;
		if(shared.buff[i]!=shared.ngetval)
			printf("error: buff[%d]=%d\n",i,shared.buff[i]);

		shared.nget++;
		shared.ngetval++;
		
		if(sem_post(&shared.mutex)==-1)
			err_sys("shared.mutex post");
		if(sem_post(&shared.mutex)==-1)
			err_sys("shared.mutex post");
		*((int*)arg)+=1;
	}	
}
