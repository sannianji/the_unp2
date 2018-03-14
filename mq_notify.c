#include "unp.h"
#include "mqueue.h"

int mq_notify(mqd_t mqd,const struct sigevent *notification)
{
	int n;
	pid_t pid;
	struct mq_hdr *mqhdr;
	struct mq_info *mqinfo;
	
	mqinfo=mqd;
	if(mqinfo->mqi_magic!=MQI_MAGIC)
	{
		errno=EBADF;
		return -1;
	}
	pid=getpid();
	if(notification==NULL)
	{
		if(mqhdr->mqh_pid==pid)
		{
			mqhdr->mqh_Jpid=0;
		}
	}
	else
	{
		if(mqhdr->mqh_pid!=0)
		{
			if(kill(mqhdr->mqh_pid,0)!=-1||errno!=ESRCH)
			{
				errno=EBUSY;
				goto err;
			}
		}
		mqhdr->mqh_pid=pid;
		mqhdr->mqh_evemnt=*notification;
	}
	pthread_mutex_unlock(&mqhdr->mqh_lock);
	return 0;
err:
	pthread_mutex_unlock(&mqhdr->mqh_lock);
	return -1;
}
