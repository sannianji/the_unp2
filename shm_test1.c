#include "unp.h"

int main(int argc,char **argv)
{
	int fd,i;
	char *ptr;
	size_t filesize,mmapsize,pagesize;
	
	if(argc!=4)
		err_quit("usage: shm_test <pathname><filesize><mmapsize>");
	filesize=atoi(argv[2]);
	mmapsize=atoi(argv[3]);

	if((fd=open(argv[1],O_RDWR|O_CREAT|O_TRUNC,FILE_MODE))<0)
		err_sys("open");
	if(lseek(fd,filesize-1,SEEK_SET)<0)
		err_sys("lseek");
	if(write(fd,"",1)!=1)
		err_sys("write");
	
	if((ptr=mmap(NULL,mmapsize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
		err_sys("mmap");

	if(close(fd)<0)
		err_sys("close");

	if(((pagesize=sysconf(_SC_PAGESIZE)))<0)
		err_sys("sysconf");

	printf("PAGESIZE =%ld\n",(long)pagesize);

	for(i=0;i<max(filesize,mmapsize);i+=pagesize)
	{
		printf("ptr[%d]=%d\n",i,ptr[i]);
		ptr[i]=1;
		printf("ptr[%d]=%d\n",i+pagesize-1,ptr[i+pagesize-1]);
		ptr[i+pagesize-1]=1;
	}
	printf("ptr[%d]=%d\n",i,ptr[i]);

	exit(0);
}
