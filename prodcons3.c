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
	int i,nthreads,count[MAXTHREADS];
	pthread_t tid_produce[MAXNTHREADS],tid_consume;

	if(argc!=3)
		err_quit("usage: prodcons3<#items><#threads>");

	nitems
	
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
