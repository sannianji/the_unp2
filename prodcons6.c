#include "unp.h"
#define MAXNITEMS 1000000
#define MAXNTHREADS 100

int nitems;
int buff[MAXNITEMS];

struct {
	pthread_mutex_t mutex;
	int nput;
	int nval;
}put={
	PTHREAD_MUTEX_INITIALIZER
};

struct {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int nready;
}nready={
	PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER
};
void *produce(void*);
void *consume(void*);

int main(int argc,char **argv)
{
	int i,nthreads,count[MAXNTHREADS];
	pthread_t tid_produce[MAXNTHREADS],tid_consume;
	int err;

	if(argc!=3)
		err_quit("usage: prodcons6 <#nitems><#nthreads>\n");
	nthreads=min(atoi(argv[2]),MAXNTHREADS);
	nitems=min(atoi(argv[1]),MAXNITEMS);

	for(i=0;i<nthreads;++i)
	{
		count[i]=0;
		err=pthread_create(&tid_produce[i],NULL,produce,&count[i]);
		if(err!=0)
			err_sys("produce pthread_create");
	}
	
	err=pthread_create(&tid_consume,NULL,consume,NULL);
	if(err!=0)
		err_sys("consume pthread_create");


	for(i=0;i<nthreads;++i)
	{
		err=pthread_join(tid_produce[i],NULL);
		if(err!=0)
			err_sys("produce pthread_join");
		printf("count[%d]=%d\n",i,count[i]);
	}

	err=pthread_join(tid_consume,NULL);
	if(err!=0)
		err_sys("consume pthread_join");
	exit(0);	
}


void *produce (void *arg)
{
	int err;
	for(;;)
	{
		err=pthread_mutex_lock(&put.mutex);
		if(err!=0)
			err_sys("pthread_mutex_lock");

		if(put.nput>=nitems)
		{
			pthread_mutex_unlock(&put.mutex);
			return NULL;
		}

		buff[put.nput]=put.nval;
		put.nput++;
		put.nval++;
		pthread_mutex_unlock(&put.mutex);


		err=pthread_mutex_lock(&nready.mutex);
		if(err!=0)
			err_sys("pthread_mutex_lock");
		if(nready.nready==0)
		{
			err=pthread_cond_signal(&nready.cond);
			if(err!=0)
				err_sys("pthread_cond_signal");
		}
		nready.nready++;
		pthread_mutex_unlock(&nready.mutex);

		*((int *)arg)+=1;
	}
}


void *consume(void *arg)
{
	int i;
	int err;
	for(i=0;i<nitems;i++)
	{
		err=pthread_mutex_lock(&nready.mutex);
		if(err!=0)
			err_sys("pthread_mutex_lock");
		while(nready.nready==0)
		{
			err=pthread_cond_wait(&nready.cond,&nready.mutex);
			if(err!=0)
				err_sys("pthread_cond_wait");
		}

		nready.nready--;
		pthread_mutex_unlock(&nready.mutex);

		if(buff[i]!=i)
			printf("buff[%d]=%d",i,buff[i]);
	}
	return NULL;
}
