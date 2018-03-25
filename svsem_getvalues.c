#include "unp.h"
int main(int argc,char **argv)
{
	int semid,nsems,i;
	int key;
	struct semid_ds seminfo;
	unsigned short *ptr;
	union semun arg;
	
	if(argc!=2)
		err_quit("usage: semgetvalues <pathname>");
	if((key=ftok(argv[1],0))<0)
		err_sys("ftok");
	if((semid=semget(key,0,0))<0)
		err_sys("semget");
	arg.buf=&seminfo;
	if(semctl(semid,0,IPC_STAT,arg)<0)
		err_sys("semctl");
	nsems=arg.buf->sem_nsems;

	if((ptr=calloc(nsems,sizeof(unsigned short)))==NULL)
		err_sys("calloc");
	arg.array=ptr;

	if(semctl(semid,0,GETALL,arg)<0)
		err_sys("semctl");
	
	for(i=0;i<nsems;i++)
		printf("semval[%d]=%d\n",i,ptr[i]);

	exit(0);
}
