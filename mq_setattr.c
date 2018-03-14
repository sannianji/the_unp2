#include "unp.h"
#include "mqueue.h"


int mq_setattr(mqd_t mqd,const struct mq_attr* mqattr,struct mq_attr* omqstat)
{
	int n;
	struct mq_hdr* mqhdr;
	struct mq_attr* attr;
	struct mq_info *mqinfo;

	mqinfo=mqd;
	if(mqinfo->mqi_magic!=MQI_MAGIC)
	{
		errno=EBADF;
		return -1;
	}

	mqhdr=mqinfo->mqi_hdr;
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
		omqstat->mq_curmsgs=attr->mq_curmsgs;
	}
	if(mqstat->mq_flags&O_NONBLOCK)
		mqinf->mqi_flags&=O_NONBLOCK;

	else
		mqinf->mqi_flags&=~O_NONBLOCK;

	pthread_mutex_unlock(&mqhdr->mqh_lock);
	return 0;
}
