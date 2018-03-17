#include "unp.h"
int main(int argc,char **argv)
{
	sem_t *sem;
	int val;
	
	if(argc!=2)
		err_quit("usage: semwait <name>\n");
	
	if((sem=sem_open(argv[1],0))==SEM_FAILED)
		err_sys("sem_open");
	if(sem_wait(sem)<0)
		err_sys("sem_wait");
	if(sem_getvalue(sem,&val)<0)
		err_sys("sem_getvalue");

	printf("pid %ld has semphore,value=%d\n",(long)getpid(),val);
	
	pause();
	exit(0);
}
