#include "unp.h"

mqd_t mqd;
struct mq_attr attr;
struct sigevent sigev;

static void notify_thread(union sigval);
static void err_quit(const char* str1)
{
	fputs(str1,stderr);
	fflush(stderr);
	exit(-1);
}
static void err_sys(const char *str)
{
	fputs(str,stderr);
	fprintf(stderr," error:%s\n",strerror(errno));
	fflush(stderr);
	exit(-1);
}

int main(int argc,char** argv)
{
	if(argc!=2)
		err_quit("usage: mqnotifythread1 <name>\n");
	if((mqd=mq_open(argv[1],O_RDONLY|O_NONBLOCK))<0)
		err_sys("mq_open");
	if(mq_getattr(mqd,&attr)<0)
		err_sys("mq_getattr");
	
	sigev.sigev_notify=SIGEV_THREAD;
	sigev.sigev_value.sival_ptr=NULL;
	sigev.sigev_notify_function=notify_thread;
	sigev.sigev_notify_attributes=NULL;
	
	if(mq_notify(mqd,&sigev)<0)
		err_sys("mq_notify");

	for(;;)
		pause();
	exit(0);
}

static void notify_thread(union sigval arg)
{
	ssize_t n;
	void *buff;
	
	printf("notify_thread start\n");
	if((buff=malloc(attr.mq_msgsize))==NULL)
		err_sys("malloc");
	if(mq_notify(mqd,&sigev)<0)
		err_sys("mq_notify");

	while((n=mq_receive(mqd,buff,attr.mq_msgsize,NULL))>=0)
	{
		printf("read %ld bytes\n",(long)n);
	}
	if(errno!=EAGAIN)
		err_sys("mq_receive error");
	free(buff);
	pthread_exit(NULL);
}
