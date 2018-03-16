#include "unp.h"
#include "pthread_rwlock.h"

int pthread_rwlock_unlock(pthread_rwlock_t *rw)
{
	int result;
	
	if(rw->magic!=RW_MAGIC)
		return EINVAL;
	if((result=pthread_mutex_lock(&rw->rw_mutex))!=0)
		return result;
	
	if(rw->rw_refcount>0)
		rw->rw_refcount--;
	else if(rw->rw_refcount==-1)
		rw->rw_refcount=0;
	else
		err_dump("rw_refcount=%d",rw->rw_refcount);

	if(rw->rw_nwaitwrites>0)
	{
		if(rw->rw_refcount==0)
			result=pthread_cond_signal(&rw->rw_condwriters);	
	}
	else if(wr->rw_nwaireaders>0)
		result=pthread_cond_broadcast(&rw->rw_condreaders);
	pthread_mutex_unlock(&rw->rw_mutex);
	return (result);
}
