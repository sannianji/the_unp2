typedef struct mq_info *mqd_t;

struct mq_attr
{
	long mq_flags;
	long mq_maxmsg;
	long mq_msgsize;
	long mq_msgcurmsgs;
};

struct mq_hdr
{
	struct mq_attr mqh_attr;
	long mqh_head;
	long mqh_free;
	long mqh_nwait;
	pid_t mqh_pid;
	struct sigevent mqh_event;
	pthread_mutex_t mqh_lock;
	pthread_cond_t mqh_wait;
};

struct msg_hdr
{
	long msg_nex;
	ssize_t msg_len;
	unsigned int msg_prio;
};

struct mq_info
{
	struct mq_hdr *mqi_hdr;
	long mqi_magic;
	int mqi_flags;

};

#define MQI_MAGIC 0x98765432

#define MSGSIZE(i) ((((i)+sizeof(long)-1)/sizeof(long))*sizeof(long))
