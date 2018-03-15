#ifndef __UNP_H
#define __UNP_H
#include <stdarg.h>
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
#include <sys/msg.h>
#include <sys/ipc.h>
#include <limits.h>
#define SERV_FIFO "/tmp/fifo.serv"
#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#define MSG_R 0400
#define MSG_W 0200
#define SVMSG_MODE (MSG_R|MSG_W|MSG_R>>3|MSG_R>>6)
#define MAXLINE 1024
#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"
#define MY_DATA 8
struct my_msgbuf;
struct msgbuf;
//typedef void Sigfunc_rt(int,siginfo_t *,void *);
//Sigfunc_rt* signal_rt(int signo,Sigfunc_rt *func,sigset_t *mask);
void err_msg(const char*,...);
void err_sys(const char *str);
void err_quit(const char *str);
#endif
