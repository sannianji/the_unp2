#include "unp.h"

int main(int argc,char **argv)
{
	int fd;
	pid_t pid;
	if((fd=open("test1.data",O_RDWR|O_CREAT,FILE_MODE))<0)
		err_sys("open");
	
	write_lock(fd,0,SEEK_SET,0);
	printf("%s:parent has write lock\n",Gf_time());
	
	if((pid=fork())<0)
		err_sys("fork");
	else if(pid==0)
	{
		sleep(3);
		printf("%s:first child tries to obtain write lock\n",Gf_time());
		writew_lock(fd,0,SEEK_SET,0);
		printf("%s:first child obtains write lock\n",Gf_time());
		sleep(8);
		un_lock(fd,0,SEEK_SET,0);
		printf("%s:first child release write lock\n",Gf_time());
		exit(0);
	}
	if((pid=fork())<0)
		err_sys("fork");
	else if(pid==0)
	{
		sleep(1);
		printf("%s:second child tries to botain read lock\n",Gf_time());
		readw_lock(fd,0,SEEK_SET,0);
		printf("%s:second child obtains read lock\n",Gf_time());
		sleep(4);
		un_lock(fd,0,SEEK_SET,0);
		printf("%s:second child releases read lock\n",Gf_time());
		exit(0);
	}
	sleep(5);
	un_lock(fd,0,SEEK_SET,0);
	printf("%s:parent releases write lock\n",Gf_time());
	exit(0);
}
