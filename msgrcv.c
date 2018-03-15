#include "unp.h"

#define MAXMSG (8192+sizeof(long))

int main(int argc,char **argv)
{
	int c,flag,mqid;
	long type;
	ssize_t n;
	struct msgbuf *buff;
	key_t key;
	type=flag=0;
	while((c=getopt(argc,argv,"nt:"))!=-1)
	{
		switch(c)
		{
			case 'n':
				flag|=IPC_NOWAIT;
				break;
			case 't':
				type=atol(optarg);
				break;
			case '?':
				err_sys("gotopt");
		}
	}
	if(optind!=argc-1)
		err_quit("usage: msgrcv [-n][-t type]<pathname>}");
	if((key=ftok(argv[optind],0))<0)
		err_sys("ftok");
	if((mqid=msgget(key,MSG_R))<0)
		err_sys("msgget");

	if((buff=malloc(MAXMSG))<0)
		err_sys("malloc");
	
	if((n=msgrcv(mqid,buff,MAXMSG,type,flag))<0)
		err_sys("msgrcv");
	printf("read %d bytes,type=%ld\n",n,buff->mtype);
	exit(0);
}
