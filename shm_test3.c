#include "unp.h"

int main(int argc,char **argv)
{
	int fd1,fd2,*ptr1,*ptr2;
	pid_t childpid;
	struct stat stat;
	
	if(argc!=2)
		err_quit("usage: test3 <name>");

	shm_unlink(argv[1]);

	if((fd1=shm_open(argv[1],O_RDWR|O_CREAT|O_EXCL,FILE_MODE))<0)
		err_sys("shm_open");
	if(ftruncate(fd1,sizeof(int))==-1)
		err_sys("ftruncate");

	if((fd2=open("the_shm_test",O_RDONLY))<0)
		err_sys("open");

	if(fstat(fd2,&stat)<0)
		err_sys("fstat");
	if((childpid=fork())<0)
		err_sys("fork");
	else if(childpid==0)
	{
		if((ptr2=mmap(NULL,stat.st_size,PROT_READ,MAP_SHARED,fd2,0))==MAP_FAILED)
			err_sys("mmap");

		if((ptr1=mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,fd1,0))==MAP_FAILED)
			err_sys("mmap");
		printf("child:shm ptr= %p,motd ptr=%p\n",ptr1,ptr2);
		sleep(5);
		printf("shared memory interger=%d\n",*ptr1);
		exit(0);
	}
	else
	{
		if((ptr1=mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,fd1,0))==MAP_FAILED)
			err_sys("mmap");

		if((ptr2=mmap(NULL,stat.st_size,PROT_READ,MAP_SHARED,fd2,0))==MAP_FAILED)
			err_sys("mmap");
		printf("parent:shm ptr=%p,motd ptr=%p\n",ptr1,ptr2);
		*ptr1=777;
		if(waitpid(childpid,NULL,0)<0)
			err_sys("waitpid");
		exit(0);
	}
}
