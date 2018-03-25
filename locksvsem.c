#include "unp.h"
#define LOCK_PATH "/tmp/svsemlock"
#define MAX_TRIES 10

int semid,initflag;
struct sembuf postop,waitop;

void my_lock(int fd)
{

	int oflag,i;
	key_t key;
	union semun arg;
	struct semid_ds seminfo;

	if(initflag==0)
	{
		oflag=IPC_CREAT|IPC_EXCL|SVSEM_MODE;
		if((key=ftok(LOCK_PATH,0))==-1)
			err_sys("ftok");
		if((semid=semget(key,1,oflag))>=0)
		{//success
			arg.val=1;
			if(semctl(semid,0,SETVAL,arg)==-1)
				err_sys("semctl");
		}
		else if(errno==EEXIST)
		{
			if((semid=semget(key,1,SVSEM_MODE))==-1)
				err_sys("semget");
			arg.buf=&seminfo;
			for(i=0;i<MAX_TRIES;i++)
			{
				if(semctl(semid,0,IPC_STAT,arg)==-1)
					err_sys("semctl");
				if(arg.buf->sem_otime!=0)
					goto init;
				sleep(1);
			}
			err_quit("semget ok,but semaphore not initialized");
		}
		else
			err_sys("semget error");
	init:
		initflag=1;
		postop.sem_num=0;
		postop.sem_op=1;
		postop.sem_flg=SEM_UNDO;
		waitop.sem_num=0;
		waitop.sem_op=-1;
		waitop.sem_flg=SEM_UNDO;
	}
	if(semop(semid,&waitop,1)==-1)
		err_sys("semop");
}
void my_unlock(int fd)
{
	if(semop(semid,&postop,1)==-1)
		err_sys("semop");
}
