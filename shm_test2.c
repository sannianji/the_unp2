#include "unp.h"

#define FILE "test.data"
#define SIZE 32768

int main(int argc,char **argv)
{
	int fd,i;
	char *ptr;
	
	if((fd=open(FILE,O_RDWR|O_CREAT|O_TRUNC,FILE_MODE))<0)
		err_sys("open");

	if((ptr=mmap(NULL,SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED)
		err_sys("mmap");
	for(i=4096;i<=SIZE;i+=4096)
	{
		printf("setting files  size to %d\n",i);
		if(ftruncate(fd,i)==-1)
			err_sys("ftruncate");
		printf("i=%d\n",i);
		printf("ptr[%d]=%d\n",i-1,ptr[i-1]);
	}

	exit(0);
}

