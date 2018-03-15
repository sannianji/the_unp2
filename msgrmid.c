#include "unp.h"
int main(int argc,char **argv)
{
	int mqid;
	key_t key;
	if(argc!=2)
		err_quit("usage: <pathname>");

	if((key=ftok(argv[1],0))<0)
		err_sys("ftok");
	if((mqid=msgget(key,0000))<0)
		err_sys("msgget");

	if(msgctl(mqid,IPC_RMID,NULL)<0)
		err_sys("msgctl");
	exit(0);
}
