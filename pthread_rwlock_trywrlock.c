#include "unp.h"
#include "pthread_rwlock.h"

int pthread_rwlock_trywrlock(pthread_rwlock_t *rw)
{
	int result;
	if(rw->rw_magic!=RW_MAGIC)
		return EINVAL;
	
	if((result=pthread_mutex_lock(rw->rw_mutex))!=0)
		return result;


	if(rw->refcount!=0)
		result=EBUSY;
	else
		rw->rw_fcount=-1;

	pthread_mutex_unlock(&rw->rw_mutex);
	return result;
}
