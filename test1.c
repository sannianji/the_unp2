#include "unp.h"

static void err_sys(const char *str)
{
	fputs(str,stderr);
	fprintf(stderr," error:%s\n",strerror(errno));
	fflush(stderr);
	exit(-1);
}
Sigfunc_rt* Signal_rt(int signo,Sigfunc_rt* func,sigset_t *mask)
{
	if(signal_rt(signo,func,mask)==((Sigfunc_rt*)SIG_ERR))
		err_sys("signal_rt");
}


static void sig_rt(int signo,siginfo_t* info,void *context);
int main(int argc,char **argv)
{
	int i,j;
	pid_t pid;
	sigset_t newset;
	union sigval val;
	
	printf("SIGRTMIN=%d,SIGRTMAX=%d\n",(int)SIGRTMIN,(int)SIGRTMAX);
	if((pid=fork())<0)
		err_sys("fork");
	else if(pid==0)
	{
		if(sigemptyset(&newset)<0)
			err_sys("sigemptyset");
		if(sigaddset(&newset,SIGRTMAX)<0)
			err_sys("sigaddset");
		if(sigaddset(&newset,SIGRTMAX-1)<0)
			err_sys("sigaddset");
		if(sigaddset(&newset,SIGRTMAX-2)<0)
			err_sys("sigaddset");
		if(sigprocmask(SIG_BLOCK,&newset,NULL)<0)
			err_sys("sigprocmask");

		Signal_rt(SIGRTMAX,sig_rt,&newset);
		Signal_rt(SIGRTMAX-1,sig_rt,&newset);
		Signal_rt(SIGRTMAX-2,sig_rt,&newset);

		sleep(6);

		if(sigprocmask(SIG_UNBLOCK,&newset,NULL)<0)
			err_sys("sigprocmask");

		sleep(3);
		exit(0);
	}
	sleep(3);
	for(i=SIGRTMAX;i>=SIGRTMAX-2;i--)
	{
		for(j=0;j<=2;j++)
		{
			val.sival_int=j;
			if(sigqueue(pid,i,val)<0)
				err_sys("sigqueue");

			printf("sent signal %d,val=%d\n",i,j);
		}
	}
	exit(0);
}

static void sig_rt(int signo,siginfo_t* info,void *context)
{
	printf("received signal #%d,code=%d,ival=%d\n",signo,info->si_code,info->si_value.sival_int);
}
