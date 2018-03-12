#include "unp.h"

int pipefd[2];
static void sig_usr1(int);
static void err_quit(const char *str1)
{
	fputs(str1,stderr);
	fflush(stderr);
	exit(-1);
}
static void err_sys(const char *str)
{
	fputs(str,stderr);
	fprintf(stderr," error:%s",strerror(errno));
	fflush(stderr);
	exit(-1);
}
int main(int argc,char **argv)
{
	int nfds;
	char c;
	fd_set rset;
	mqd_t mqd;
	void *buff;
	ssize_t n;
	struct mq_attr attr;
	struct sigevent sigev;
	
	if(argc!=2)
		err_quit("usage: mqnotifysig5 <name>");
	if((mqd=mq_open(argv[1],O_RDONLY|O_NONBLOCK))<0)
		err_sys("mq_open");
	if(mq_getattr(mqd,&attr)<0)
		err_sys("mq_getattr");
	if((buff=malloc(attr.mq_msgsize))==NULL)
		err_sys("malloc");
	
	if(pipe(pipefd)<0)
		err_sys("pipe");

	if(signal(SIGUSR1,sig_usr1)==SIG_ERR)
		err_sys("signal");
	sigev.sigev_notify=SIGEV_SIGNAL;
	sigev.sigev_signo=SIGUSR1;
	
	if(mq_notify(mqd,&sigev)<0)
		err_sys("mq_notify");

	FD_ZERO(&rset);
	for(;;)
	{
		FD_SET(pipefd[0],&rset);
		again:
		if((nfds=select(pipefd[0]+1,&rset,NULL,NULL,NULL))<0)
		{
			if(errno==EINTR)
				goto again;
			else
				err_sys("select");
		}
		if(FD_ISSET(pipefd[0],&rset))
		{
			if(read(pipefd[0],&c,1)<0)
				err_sys("read");
			if(mq_notify(mqd,&sigev)<0)
				err_sys("mq_notify");

			while((n=-mq_receive(mqd,buff,attr.mq_msgsize,NULL))>=0)
				printf("read %ld bytes\n",(long)n);
		
			if(errno!=EAGAIN)
				err_sys("mq_receive error");
		}
	}
	exit(0);
}

static void sig_usr1(int signo)
{
	if(write(pipefd[1],"",1)!=1)
		err_sys("write");
	return;
}
