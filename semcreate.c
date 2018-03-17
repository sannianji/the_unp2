#include "unp.h"

int main(int argc,char **argv)
{
	int c,flags;
	sem_t *sem;
	unsigned int value;
	
	flags=O_RDWR|O_CREAT;
	value=1;
	while((c=getopt(argc,argv,"ei:"))!=-1)
	{
		switch (c)
		{
			case 'e':
				flags|=O_EXCL;
				break;
			case 'i':
				value=atoi(optarg);
				break;
		}
	}
	if(optind!=argc-1)
		err_quit("usage: semcreate [-e][-i initialvalue] <name>\n");
	
	if((sem=sem_open(argv[optind],flags,FILE_MODE,value))==SEM_FAILED)
		err_sys("sem_open");

	if(sem_close(sem)<0)
		err_sys("sem_open");

	exit(0);
}
