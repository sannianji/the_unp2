#include "unp.h"

int main(int argc,char **argv)
{
	int semid;
	int key;
	if(argc!=2)
		err_quit("usage: semrmid <pathname>");
	if((key=ftok(argv[1],0))<0)
		err_sys("ftok");
	if((semid=semget(key,0,0))==-1)
		err_sys("semget");
	if(semctl(semid,0,IPC_RMID)==-1)
		err_sys("semctl");
	exit(0);
	
}
