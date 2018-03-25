#include "unp.h"

int main(int argc,char **argv)
{
	int fd,c,flags;
	char *ptr;
	off_t length;
	
	flags=O_RDWR|O_CREAT;
	while((c=getopt(argc,argv,"e"))!=-1)
	{
		switch(c)
		{
			case 'e':
				flags|=O_EXCL;
				break;
			case '?':
				err_sys("getopt");
		}
	}
	if(optind!=argc-2)
		err_quit("usage: shmcreate [-e]<name><length>");
	length=atoi(argv[optind+1]);

	if((fd=shm_open(argv[optind],flags,FILE_MODE))==-1)
		err_sys("shm_open");
	if(ftruncate(fd,length)==-1)
		err_sys("ftruncate");
	
	if((ptr=mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
		err_sys("mmap");
	
	exit(0);
}
