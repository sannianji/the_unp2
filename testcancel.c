#include "unp.h"
#include "pthread_rwlock.h"
pthread_rwlock_t rwlock=PTHREAD_RWLOCK_INITIALIZER;
pthread_t tid1,tid2;
void *thread1(void*),*thread2(void*);

int main(int argc,char **argv)
{
	void *status;
	int err;
	err=pthread_create(&tid1,NULL,thread1,NULL);
	if(err!=0)
	{
		errno=err;
		err_sys("tid1 pthread_create");
	}
	sleep(1);
	
	err=pthread_create(&tid2,NULL,thread2,NULL);
	if(err!=0)
	{
		errno=err;
		err_sys("tid2 pthread_create");
	}
	err=pthread_join(tid2,&status);
	if(err!=0)
	{
		errno=err;
		err_sys("tid2 pthread_create");
	}
	if(status!=PTHREAD_CANCELED)
		printf("thread2 status=%p\n",status);
	err=pthread_join(tid1,&status);
	if(err!=0)
	{
		errno=err;
		err_sys("tid1 pthread_join");
	}
	if(status!=NULL)
		printf("thread1 status=%p\n",status);
	
	printf("rw_refcount=%d,rw_nwaitreaders=%d,rw_nwaitwriters=%d\n",rwlock.rw_refcount,rwlock.rw_nwaitreaders,rwlock.rw_nwaitwriters);

	err=pthread_rwlock_destroy(&rwlock);
	exit(0);
}

void *thread1(void *arg)
{
	int err=pthread_rwlock_rdlock(&rwlock);
	if(err!=0)
	{
		errno=err;
		err_sys("pthread_rwlock_rdlock");
	}
	printf("thread1 got a read lock\n");
	sleep(3);
	pthread_cancel(tid2);
	sleep(3);
	err=pthread_rwlock_unlock(&rwlock);
	if(err!=0)
	{
		errno=err;
		err_sys("pthread_rwlock_unlock");
	}
	return NULL;
}


void * thread2(void *arg)
{
	int err;
	printf("thread2() trying to obtain a write lock\n");
	if((err=pthread_rwlock_wrlock(&rwlock))!=0)
	{
		errno=err;
		err_sys("pthread_rwlock_wrlock");
	}
	printf("thread2() got a write lock\n");
	sleep(1);
	if((err=pthread_rwlock_unlock(&rwlock))!=0)
	{
		errno=err;
		err_sys("pthread_rwlock_unlock");
	}
	return NULL;
}

