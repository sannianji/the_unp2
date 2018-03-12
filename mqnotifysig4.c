#include "unp.h"
static void err_quit(const char *str)
{
	fputs(str,stderr);
	fflush(stderr);
	exit(-1);
}
static void err_sys(const char *str)
{
	fputs(str,stderr);
	fprintf(stderr," error: %s",strerror(errno));	
	
	fflush(stderr);
	exit(-1);
}
static void Sigwait(sigset_t* the_sigset,int* signo)
{
	int n;
	if((n=sigwait(the_sigset,signo))==0)
		return;
	errno=n;
	err_sys("sigwait");
}

int main(int argc,char **argv)
{
	int signo;
	mqd_t mqd;
	void *buff;
	ssize_t n;
	sigset_t newmask;
	struct mq_attr attr;
	struct sigevent sigev;
	
	if(argc!=2)
		err_quit("usage: mqnotifysig4 <name>\n");
	if((mqd=mq_open(argv[1],O_RDONLY|O_NONBLOCK))<0)
		err_sys("mq_open");
	if(mq_getattr(mqd,&attr)<0)
		err_sys("mq_getattr");
	if((buff=malloc(attr.mq_msgsize))==NULL)
		err_sys("malloc");
	
	if(sigemptyset(&newmask)<0)
		err_sys("sigemptyset");
	if(sigaddset(&newmask,SIGUSR1)<0)
		err_sys("sigaddset");
	if(sigprocmask(SIG_BLOCK,&newmask,NULL)<0)
		err_sys("sigprocmask");
	
	sigev.sigev_notify=SIGEV_SIGNAL;
	sigev.sigev_signo=SIGUSR1;
	if(mq_notify(mqd,&sigev)<0)
		err_sys("mq_notify");
	
	for(;;)
	{
		if((errno=sigwait(&newmask,&signo))!=0)
			err_sys("sigwait");
		if(signo==SIGUSR1)
		{
			if(mq_notify(mqd,&sigev)<0)
				err_sys("mq_notify");
			while((n=mq_receive(mqd,buff,attr.mq_msgsize,NULL))>=0)
				printf("read %ld bytes\n",(long)n);

			if(errno!=EAGAIN)
				err_sys("mqreceive");

		}

	}
	exit(0);
}
