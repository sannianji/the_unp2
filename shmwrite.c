#include "unp.h"

int main(int argc,char **argv)
{
	int i,fd;
	struct stat stat;
	unsigned char *ptr;
	
	if(argc!=2)
		err_quit("usage: shmwrite<name>");
	
	if((fd=shm_open(argv[1],O_RDWR,FILE_MODE))==-1)
		err_sys("shm_open");

	if(fstat(fd,&stat)==-1)
		err_sys("fstat");
	if((ptr=mmap(NULL,stat.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
		err_sys("mmap");
	if(close(fd)<0)
		err_sys("close");
	
	for(i=0;i<stat.st_size;i++)
		*ptr++=i%256;
	exit(0);	
}
