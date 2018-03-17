#include "unp.h"
int main(int argc,char **argv)
{
	sem_t *sem;
	int val;
	
	if(argc!=2)
		err_quit("usage: semgetvalue <name>\n");
	
	if((sem=sem_open(argv[1],0))==SEM_FAILED)
		err_sys("sem_open");
	if(sem_getvalue(sem,&val)<0)
		err_sys("sem_getvalue");
	printf("value=%d\n",val);
	exit(0);
}
