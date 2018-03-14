#include "unp.h"
#include "mqueue.h"

int mq_getattr(mqd_t mqd,struct mq_attr *mqstat)
{
	int n;
	struct mq_hdr *mqhdr;
	struct mq_attr *attr;
	struct mq_info *mqinfo;
	
	mqinfo=mqd;
	if(mqinfo->mq_magic!=MQI_MAGIC)
	{
		errno=EBADF;
		return -1;
	}
	
	mqhdr=mqinfo->mq_hdr;
	attr=&mqhdr->mq_attr;

	if((n=pthread_mutex_lock(&mqhdr->mqh_lock))!=0)
	{
		errno=n;
		return -1;
	}
	if(omqstat!=NULL)
	{
		omqstat->mq_flags=mqinfo->mqi_flags;
		omqstat->mq_maxmsg=attr->mq_maxmsg;
		omqstat->mq_msgsize=attr->mq_msgsize;
		mqstat->mq_curmsgs=attr->curmsgs;

		pthread_mutex_unlock(&mqhdr->mqh_lock);
		return 0;
	}
}
