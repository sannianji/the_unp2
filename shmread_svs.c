#include "unp.h"
int main(int argc,char **argv)
{
	int i,id;
	struct shmid_ds buff;
	unsigned char c,*ptr;
	key_t key;
	if(argc!=2)
		err_sys("usage: shmread <pathname>");
	if((key=ftok(argv[1],0))==-1)
		err_sys("ftok");
	
	if((id=shmget(key,0,SVSHM_MODE))==-1)
		err_sys("shmget");
	if((ptr=shmat(id,NULL,0))==-1)
		err_sys("shmat");
	if((shmctl(id,IPC_STAT,&buff))==-1)
		err_sys("shmctl");
	for(i=0;i<buff.shm_segsz;i++)
		if((c=*ptr++)!=(i%256))
			err_ret("ptr[%d]=%d",i,c);
	
	exit(0);
}
