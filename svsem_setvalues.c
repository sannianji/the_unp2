#include "unp.h"
int main(int argc,char **argv)
{
	int semid,nsems,i;
	struct semid_ds seminfo;
	unsigned short *ptr;
	union semun arg;
	int key;
	if(argc<2)
		err_quit("usage: semsetvalues <pathname> [values ...]");
	if((key=ftok(argv[1],0))<0)
		err_sys("ftok");
	if((semid=semget(key,0,0))==-1)
		err_sys("semget");

	arg.buf=&seminfo;
	if(semctl(semid,0,IPC_STAT,arg)==-1)
		err_sys("semctl");
	nsems=arg.buf->sem_nsems;

	if(argc!=nsems+2)
		err_quit("%d semaphores in set,%d values specified",nsems,argc-2);

	if((ptr=calloc(nsems,sizeof(unsigned short)))==NULL)
		err_sys("calloc");

	arg.array=ptr;
	for(i=0;i<nsems;i++)
		ptr[i]=atoi(argv[i+2]);
	if(semctl(semid,0,SETALL,arg)==-1)
		err_sys("semctl");
	exit(0);
	
}
