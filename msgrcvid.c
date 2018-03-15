#include "unp.h"

#define MAXMSG (8192+sizeof(long))

int main(int argc,char **argv)
{
	int mqid;
	ssize_t n;
	struct msgbuf *buff;

	if(argc!=2)
		err_quit("usage: msgrcvid <mqid>");

	mqid=atoi(argv[1]);

	if((buff=malloc(MAXMSG))==NULL)
		err_sys("malloc");

	if((n=msgrcv(mqid,buff,MAXMSG,0,0))<0)
		err_sys("msgrcv");

	printf("read %d bytes,types =%ld\n",n,buff->mtype);
	exit(0);
}
