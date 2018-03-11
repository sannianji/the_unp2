#include "unp.h"
static void err_quit(const char *str)
{
	printf(str);
	fflush(stdout);
	exit(-1);
}
int main(int argc,char **argv)
{
	int c,flags;
	mqd_t mqd;
	
	flags=O_RDWR|O_CREAT;
	while((c=getopt(argc,argv,"e"))!=-1)
	{
		switch(c)
		{
			case 'e':
				flags|=O_EXCL;
				break;
		}
	}
	if(optind!=argc-1)
		err_quit("usage: mqcreate [-e] <name>");

	mqd=mq_open(argv[optind],flags,FILE_MODE,NULL);

	mq_close(mqd);
	exit(0);
}
