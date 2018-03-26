#include "unp.h"
int main(int argc,char **argv)
{
	int c,oflag,semid,nsems;
	int key;
	oflag=SVSEM_MODE|IPC_CREAT;
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
		err_quit("usage: svsem_create [-e] <pathname> <nsems>");

	nsems=atoi(argv[optind+1]);
	if((key=ftok(argv[optind],0))<0)
		err_sys("ftok");

	if((semid=semget(key,nsems,oflag))==-1)
		err_sys("semget");
	
	exit(0);
}
