
#include "unp.h"
#define MAXNITEMS 100000
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
	int err;
	pthread_t tid_produce[MAXNTHREADS],tid_consume;

	if(argc!=3)
		err_quit("usage:prodcons2 <#items><#threads>\n");

	nitems=min(atoi(argv[1]),MAXNITEMS);
	nthreads=min(atoi(argv[2]),MAXNTHREADS);

//	pthread_setconcurrency(nthreads);
	for(i=0;i<nthreads;i++)
	{
		count[i]=0;
		if((err=pthread_create(&tid_produce[i],NULL,produce,&count[i]))!=0)
		{
			errno=err;
			err_sys("tid_produce pthread_create");
		}
	}
	for(i=0;i<nthreads;i++)
	{
		int n;
		if((n=pthread_join(tid_produce[i],NULL))!=0)
		{
			errno=n;
			err_sys("tid_produce pthread_join");
		}
		printf("count[%d]=%d\n",i,count[i]);
	}

	if(pthread_create(&tid_consume,NULL,consume,NULL)<0)
		err_sys("tid_consume pthread_create");
	int n;
	if((n=pthread_join(tid_consume,NULL))!=0)
	{
		errno=n;
		err_sys("tid_consume pthread_join");
	}
	exit(0);
	
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


void* consume(void *arg)
{
	int i;
	for(i=0;i<nitems;i++)
	{
		if(shared.buff[i]!=i)
			printf("buff[%d]=%d\n",i,shared.buff[i]);
	}
	return NULL;
}
