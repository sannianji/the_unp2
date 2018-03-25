#include "unp.h"

int main(int argc,char **argv)
{
	int c,i,flag,semid,nops,key;
	struct sembuf *ptr;
	flag=0;
	while((c=getopt(argc,argv,"nu"))!=-1)
	{
		switch(c)
		{
			case 'n':
				flag|=IPC_NOWAIT;
				break;
			case 'u':
				flag|=SEM_UNDO;
				break;
			case '?':
				err_sys("getopt");
		}
	}
	if(argc-optind<2)
		err_quit("usage: semops [-n][-u]<pathname> operation ...");
	if((key=ftok(argv[1],0))==-1)
		err_sys("ftok");
	if((semid=semget(key,0,0))==-1)
		err_sys("semget");
	optind++;
	nops=argc-optind;
	if((ptr=calloc(nops,sizeof(struct sembuf)))==NULL)
		err_sys("calloc");
	for(i=0;i<nops;i++)
	{
		ptr[i].sem_num=i;
		ptr[i].sem_op=atoi(argv[optind+i]);
		ptr[i].sem_flg=flag;
	}
	if(semop(semid,ptr,nops)==-1)
		err_sys("semop");
	
	exit(0);
}
