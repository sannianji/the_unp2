#ifndef __UNP_H
#define __UNP_H
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <signal.h>
#include <sys/select.h>
#include <pthread.h>
#define SERV_FIFO "/tmp/fifo.serv"
#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#define MAXLINE 1024
#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"
typedef void Sigfunc_rt(int,siginfo_t *,void *);

Sigfunc_rt* signal_rt(int signo,Sigfunc_rt *func,sigset_t *mask)
{
	struct sigaction act,oact;
	
	act.sa_sigaction=func;
	act.sa_mask=*mask;
	act.sa_flags=SA_SIGINFO;
	if(signo==SIGALRM)
	{
#ifdef SA_INTERRUPT
	act.sa_flags|=SA_INTERRUPT;
#endif
	}else
	{
#ifdef SA_RESTART
	act.sa_flags|=SA_RESTART;
#endif
	}
	if(sigaction(signo,&act,&oact)<0)
		return ((Sigfunc_rt*)SIG_ERR);
	return (oact.sa_sigaction);
}
#endif
