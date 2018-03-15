#include "unp.h"
static void err_sys(const char *str)
{
	fputs(str,stderr);
	fprintf(stderr," error: %s\n",strerror(errno));
	exit(-1);
}

static void err_quit(const char *str)
{
	fputs(str,stderr);
	exit(-1);
}
int main(int argc,char **argv)
{
	int c,oflag,mqid;
	key_t key;
	oflag=SVMSG_MODE|IPC_CREAT;
	while((c=getopt(argc,argv,"e"))!=-1)
	{
		switch (c)
		{
			case 'e':
				oflag|=IPC_EXCL;
				break;
			case '?':
				exit(-1);
		}
	}
	if(optind!=argc-1)
		err_quit("usage: msgcreate [-e] <pathname>\n");
	if((key=ftok(argv[optind],0))<0)
		err_sys("ftok");
	if((mqid=msgget(key,oflag))<0)
		err_sys("msgget");

	exit(0);
}
