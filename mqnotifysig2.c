#include "unp.h"

volatile sig_atomic_t mqflag;

static void sig_usr1(int signo)
{
	mqflag=1;
	return;
}
static void err_sys(const char *str1,const char *str2)
{
	printf(str1,str2);
	fflush(stdout);
	exit(-1);
}
static void err_quit(const char *str)
{
	fputs(str,stdout);
	fflush(stdout);
	exit(-1);
}
static void Sigemptyset(sigset_t *the_sigset)
{
	if(sigemptyset(the_sigset)<0)
		err_sys("sigemptyset error",strerror(errno));
}
int main(int argc,char **argv)
{
	mqd_t mqd;
	void *buff;
	ssize_t n;
	sigset_t zeromask,newmask,oldmask;
	struct mq_attr attr;
	struct sigevent sigev;
	
	if(argc!=2)
		err_quit("usage: mqnotifysig2 <name>\n");
	if((mqd=mq_open(argv[1],O_RDONLY))<0)
		err_sys("mq_open error: %s\n",strerror(errno));
	if(mq_getattr(mqd,&attr)<0)
		err_sys("mq_getattr error: %s\n",strerror(errno));
	
	if((buff=malloc(attr.mq_msgsize))==NULL)
		err_sys("malloc error: %s\n",strerror(errno));
	
	Sigemptyset(&zeromask);
	Sigemptyset(&newmask);
	Sigemptyset(&oldmask);
	if(sigaddset(&newmask,SIGUSR1)<0)
		err_sys("sigaddset error: %s\n",strerror(errno));
	
	if(signal(SIGUSR1,sig_usr1)==SIG_ERR)
		err_sys("signal error: %s\n",strerror(errno));
	sigev.sigev_notify=SIGEV_SIGNAL;
	sigev.sigev_signo=SIGUSR1;
	if(mq_notify(mqd,&sigev)<0)
		err_sys("mq_notify error: %s\n",strerror(errno));

	for(;;)
	{
		if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
			err_sys("sigprocmask error: %s\n",strerror(errno));
	
		while(mqflag==0)
			sigsuspend(&zeromask);
		mqflag=0;
	
		if(mq_notify(mqd,&sigev)<0)
			err_sys("mq_notify error:%s\n",strerror(errno));

		if((n=mq_receive(mqd,buff,attr.mq_msgsize,NULL))<0)
			err_sys("mq_receive error:%s\n",strerror(errno));
		printf("read %ld bytes\n",(long)n);
		if(sigprocmask(SIG_UNBLOCK,&newmask,NULL)<0)
			err_sys("sigprocmask error:%s\n",strerror(errno));
	}
	exit(0);
}
