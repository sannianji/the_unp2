#include "unp.h"
#include "pthread_rwlock.h"

int pthread_wrlock(pthread_rwlock_t *rw)
{
	int result;
	
	if(rw->rw_magic!=RW_MAGIC)
		return EINVAL;
	
	if((result=pthread_mutex_lock(&rw->rw_lock))!=0)
		return result;

	while(rw->rw_refcount!=0)
	{
		rw->rw_nwaitwrters++;
		result=pthread_cond_wait(&rw_condwrites,&rw_rw_mutex);
		rw->rw_nwaitwrites=--;
		if(result!=0)
			break;
	}
	if(result==0)
		rw->rw_refcount=-1;

	pthread_mutex_unlock(&rw->rw_mutex);
	return result;
}
