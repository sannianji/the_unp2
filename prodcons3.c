#include "unp.h"
#define MAXNITEMS 1000000
#include <pthread.h>
#define MAXNTHREADS 100
int nitems;

struct {
	pthread_mutex_t mutex;
	int buff[MAXNITEMS];
	int nput;
	int nval;
}shared={
	PTHREAD_MUTEX_INITIALIZER
};

void *produce(void*),*consume(void *);


int main(int argc,char **argv)
{
	int i,nthreads,count[MAXNTHREADS];
	pthread_t tid_produce[MAXNTHREADS],tid_consume;
	int err;
	if(argc!=3)
		err_quit("usage: prodcons3<#items><#threads>");

	nitems=min(atoi(argv[1]),MAXNITEMS);
	nthreads=min(atoi(argv[2]),MAXNTHREADS);

	//set_concurrency(nthreads+1)
	for(i=0;i<nthreads;i++)
	{
		count[i]=0;
		err=pthread_create(&tid_produce[i],NULL,produce,&count[i]);
		if(err!=0)
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
	for(i=0;i<nthreads;i++)
	{
		err=pthread_join(tid_produce[i],NULL);
		if(err!=0)
		{
			errno=err;
			err_sys("produce pthread_join");
		}
		printf("count[%d]=%d\n",i,count[i]);
	}
	err=pthread_join(tid_consume,NULL);
	if(err!=0)
	{
		errno=err;
		err_sys("consume pthread_join");
	}
}

void *produce(void *arg)
{
	int n;
	for(;;)
	{
		if((n=pthread_mutex_lock(&shared.mutex))!=0)
		{
			errno=n;
			err_sys("pthread_mutex_lock");
		}
		if(shared.nput>=nitems)
		{
			if((n=pthread_mutex_unlock(&shared.mutex))!=0)
			{
				errno=n;
				err_sys("pthread_mutex_unlock");
			}
			return(NULL);
		}
		shared.buff[shared.nput]=shared.nval;
		shared.nput++;
		shared.nval++;
		if((n=pthread_mutex_unlock(&shared.mutex))!=0)
		{
			errno=n;
			err_sys("pthread_mutex_unlock");
		}
		*((int *)arg)+=1;
	}
}

void consume_wait(int i)
{
	int err;
	for(;;)
	{
		err=pthread_mutex_lock(&shared.mutex);
		if(i<shared.nput)
		{
			err=pthread_mutex_unlock(&shared.mutex);
			return;
		}
		pthread_mutex_unlock(&shared.mutex);
	}
}
void* consume(void *arg)
{
	int i;
	for(i=0;i<nitems;i++)
	{
		consume_wait(i);
		if(shared.buff[i]!=i)
			printf("buff[%d]=%d\n",i,shared.buff[i]);
	}
	return NULL;
}
