#include "unp.h"
int main(int argc,char **argv)
{
	int mqid;
	size_t len;
	long type;
	struct msgbuf *ptr;
	key_t key;
	if(argc!=4)
		err_quit("usage:msgsnd <pathname> <#bytes><type>");
	len=atoi(argv[2]);
	type=atoi(argv[3]);
	if((key=ftok(argv[1],0))<0)
		err_sys("ftok");
	if((mqid=msgget(key,MSG_W))<0)
		err_sys("msgget");
	

	if((ptr=calloc(sizeof(long)+len,sizeof(char)))<0)
		err_sys("calloc");

	ptr->mtype=type;

	if(msgsnd(mqid,ptr,len,0)<0)
		err_sys("msgsnd");

	exit(0);
}
