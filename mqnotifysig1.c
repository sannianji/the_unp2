#include "unp.h"

mqd_t mqd;
void *buff;
struct mq_attr attr;
struct sigevent sigev;

static void sig_usr1(int);
static void err_sys(const char *str1,const char *str2)
{
	printf(str1,str2);
	fflush(stdout);	
	exit(-1);
}
static void err_quit(const char *str1)
{
	fputs(str1,stdout);
	fflush(stdout);
	exit(-1);
}
int main(int argc,char **argv)
{
	if(argc!=2)
		err_quit("usage: mqnotifysig1 <name>\n");

	if((mqd=mq_open(argv[1],O_RDONLY))<0)
		err_sys("mq_open error: %s\n",strerror(errno));
	if(mq_getattr(mqd,&attr)<0)
		err_sys("mq_getattr error: %s\n",strerror(errno));
	if((buff=malloc(attr.mq_msgsize))==NULL)
		err_sys("malloc error: %s\n",strerror(errno));
	
	if(signal(SIGUSR1,sig_usr1)==SIG_ERR)
		err_sys("signal error: %s\n",strerror(errno));
	
	sigev.sigev_notify=SIGEV_SIGNAL;
	sigev.sigev_signo=SIGUSR1;
	if(mq_notify(mqd,&sigev)<0)
		err_sys("mq_notify error: %s\n",strerror(errno));
	for(;;)
		pause();
	
	exit(0);
}

static void sig_usr1(int signo)
{
	ssize_t n;
	if(mq_notify(mqd,&sigev)<0)
		err_sys("mq_notify error: %s",strerror(errno));
	
	if((n=mq_receive(mqd,buff,attr.mq_msgsize,NULL))<0)
		err_sys("mq_recieve error: %s",strerror(errno));
	printf("SIGUSR1 recieved,read %ld bytes\n",(long) n);
	return;
}
