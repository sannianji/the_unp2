#include "unp.h"
int main(int argc,char **argv)
{
	int c,id,oflag;
	char *ptr;
	size_t length;
	key_t key;
	oflag=SVSHM_MODE|IPC_CREAT;
	while((c=getopt(argc,argv,"e"))!=-1)
	{
		switch(c)
		{
			case 'e':
				oflag|=IPC_EXCL;
				break;
			case '?':
				err_sys("getopt");
		}
	}
	if(optind!=argc-2)
		err_quit("usage: shmget [-e]<pathname><length>");
	length=atoi(argv[optind+1]);
	
	if((key=ftok(argv[optind],0))<0)
		err_sys("ftok");
	if((id=shmget(key,length,oflag))==-1)
		err_sys("shmget");
	if((ptr=shmat(id,NULL,0))==-1)
		err_sys("shmat");
	exit(0);
}
