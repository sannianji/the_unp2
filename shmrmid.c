#include "unp.h"
int main(int argc,char **argv)
{
	int id;
	key_t key;
	if(argc!=2)
		err_quit("usage:shmrmid <pathname>");
	if((key=ftok(argv[1],0))==-1)
		err_sys("ftok");
	if((id=shmget(key,0,SVSHM_MODE))==-1)
		err_sys("shmget");
	if(shmctl(id,IPC_RMID,NULL)==-1)
		err_sys("shmctl");
	exit(0);
}
