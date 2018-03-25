#include "unp.h"
int min(int n1,int n2)
{
	return (n1>n2)?n2:n1;
}
int max(int n1,int n2)
{
	return (n1>n2)?n1:n2;
}
typedef struct msgbuf
{
	long mtype;
	char mtext[1];
}Message;

struct my_msgbuf
{
	long mtype;
	int16_t mshort;
	char mchar[MY_DATA];
};
void err_msg(const char *str,...)
{
	va_list ap;
	va_start(ap,str);
	char buf[MAXLINE];
	vsnprintf(buf,sizeof(buf),str,ap);
	va_end(ap);
	fflush(stdout);
	fputs(buf,stderr);
	fflush(stderr);
	exit(-1);
}

void err_sys(const char *str,...)
{
	va_list ap;
	va_start(ap,str);
	char buf[MAXLINE];
	vsnprintf(buf,sizeof(buf),str,ap);
	va_end(ap);
	fputs(buf,stderr);
	fprintf(stderr," error:%s\n",strerror(errno));
	exit(-1);
}

void err_quit(const char* str,...)
{
	va_list ap;
	va_start(ap,str);
	char buf[MAXLINE];
	vsnprintf(buf,sizeof(buf),str,ap);
	va_end(ap);
	fputs(buf,stderr);
	fputs("\n",stderr);
	exit(-1);
}
