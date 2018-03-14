#include "unp.h"
#include "mqueue.h"

int mq_send(mqd_t mqd,const char *pthr,size_t len,unsigned int prio)
{
	int n;
	long index,freeindex;
	int8_t *mptr;
	struct sigevent *sigev;
	struct mq_hdr* mqhdr;
	struct mq_attr* attr;
	struct msg_hdr *msghdr,*nmsghdr,*pnmsghdr;
	struct mq_info *mqinfo;


	mqinfo=mqd;
	if(mqinfo->mqi_magic!=MQI_MAGIC)
	{
		errno=EBADF;
		return -1;
	}

	mqhdr=mqinfo->mq_hdr;
	mptr=(int8_t *)mqhdr;
	attr=&mqhdr->mqh_attr;

	if((n=pthread_mutex_lock(&mqhdr->mqh_lock))!=0)
	{
		errno=n;
		return -1;
	}
	if(len>attr->mq_msgsize)
	{
		errno=EMSGSIZE;
		goto err;
	}
	if(attr->mq_curmsgs==0)
	{
		if(mqhdr->mqh_pid!=0&&mqhdr->mqh_nwait==0)
		{
			sigev=&mqhdr->mqh_event;
			if(sigev->sigev_notify==SIGEV_SIGNAL)
				sigqueue(mqhdr->mqh_pid,sigev->sigev_signo,sigev->sigev_value);
		}
		mqhdr->mqh_pid=0;
	}
	else if(attr->mq_curmsgs>=attr->mq_maxmsg)
	{
		if(mqinfo->mqi_flags&O_NONBLOCK)
		{
			errno=EAGAIN;
			goto err;
		}
		while(attr->mq_curmsgs>=attr->mq_maxmsg)
			pthread_cond_wait(&mqhdr->mqh_wait,&mqhdr->mqh_lock);
	}

	if((freeindex=mqhdr->mqh_free)==0)
		err_dump("mq_send: curmsgs=%ld,free=0",attr->mq_curmsgs);

	nmsghdr=(*struct msg_hdr *)&mptr
}
