#ifndef __UNP_H
#define __UNP_H
#include <semaphore.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>
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
#define BUFFSIZE 1024
struct my_msgbuf;
struct msgbuf;
int min(int,int);
//typedef void Sigfunc_rt(int,siginfo_t *,void *);
//Sigfunc_rt* signal_rt(int signo,Sigfunc_rt *func,sigset_t *mask);
void err_msg(const char*,...);
void err_sys(const char *str,...);
void err_quit(const char *str,...);
int lock_reg(int,int,int,off_t,int,off_t);
pid_t lock_test(int,int,off_t,int,off_t);
char *Gf_time(void);
#define read_lock(fd,offset,whence,len) lock_reg(fd,F_SETLK,F_RDLCK,offset,whence,len)
#define readw_lock(fd,offset,whence,len) lock_reg(fd,F_SETLKW,F_RDLCK,offset,whence,len)
#define write_lock(fd,offset,whence,len) lock_reg(fd,F_SETLK,F_WRLCK,offset,whence,len)
#define writew_lock(fd,offset,whence,len) lock_reg(fd,F_SETLKW,F_WRLCK,offset,whence,len)
#define un_lock(fd,offset,whence,len) lock_reg(fd,F_SETLK,F_UNLCK,offset,whence,len)
#define is_read_lockable(fd,offset,whence,len) !lock_test(fd,F_RDLCK,offset,whence,len)
#define is_write_lockable(fd,offset,whence,len) !lock_test(fd,F_WRLCK,offset,whence,len)
#endif
