#include "unp.h"
#include "pthread_rwlock.h"
static cancelrdwait(void *arg)
{
	pthread_rwlock_t *rw;

	rw=arg;
	rw->rw_nwaitreaders--;
	pthread_mutex_unlock(&rw->rw_mutex);
}
int pthread_rwlock_rdlock(pthread_rwlock_t *rw)
{
	int result;
	
	if(rw->rw_magic!=RW_MAGIC)
		return EINVAL;
	
	if((result=pthread_mutex_lock(&rw->rw_mutex))!=0)
		return result;
	
	while(rw->rw_refcount<0||rw->rw_nwaitwriters>0)
	{
		rw->rw_nwaitreaders++;
		pthread_cleanup_push(rwlock_cancelrdwait,(void *)rw);
		result=pthread_cond_wait(&rw->rw_condreaders,&rw->rw_mutex);
		pthread_cleanup_pop(0);
		rw->rw_nwaitreaders--;
		if(result!=0)
			break;
	}

	if(result==0)
		rw->rw_refcount++;

	pthread_mutex_unlock(&rw->rw_mutex);
	return result;
}
