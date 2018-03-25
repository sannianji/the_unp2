#include "unp.h"

int main(int argc,char **argv)
{
	int i,fd;
	struct stat stat;
	unsigned char c,*ptr;
	
	if(argc!=2)
		err_quit("usage:shmread<name>");

	if((fd=shm_open(argv[1],O_RDONLY,FILE_MODE))==-1)
		err_sys("shm_open");
	if(fstat(fd,&stat)<0)
		err_sys("fstat");
	if((ptr=mmap(NULL,stat.st_size,PROT_READ,MAP_SHARED,fd,0))==MAP_FAILED)
		err_sys("mmap");
	
	if(close(fd)<0)
		err_sys("close");
	
	for(i=0;i<stat.st_size;i++)
	{
//		if((c=*ptr++)!=(i%256))
//			err_ret("ptr[%d]=%d",i,c);
		printf("ptr[%d]=%d\n",i,ptr[i]);
	}
	exit(0);
}
