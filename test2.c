#include "unp.h"
int main(int argc,char **argv)
{
	int fd;
	int pid;
	if((fd=open("test1.data",O_RDWR|O_CREAT,FILE_MODE))<0)
		err_sys("open");

	if(read_lock(fd,0,SEEK_SET,0)<0)
		err_sys("read_lock");

	printf("%s:parent has read lock\n",Gf_time());
	
	if((pid=fork())<0)
		err_sys("fork");
	if(pid==0)
	{
		sleep(1);
		printf("%s:first child tries to obtain write lock\n",Gf_time());
		writew_lock(fd,0,SEEK_SET,0);
		printf("%s:first child obtains write lock\n",Gf_time());
		sleep(2);
		un_lock(fd,0,SEEK_SET,0);
		printf("%s:first child release write lock\n",Gf_time());
		exit(0);
	}
	if((pid=fork())<0)
		err_sys("fork");
	if(pid==0)
	{
		sleep(3);
		printf("%s:second child tries to obtain read lock\n",Gf_time());
		readw_lock(fd,0,SEEK_SET,0);
		printf("%s:second child obtains read lock\n",Gf_time());
		sleep(4);
		un_lock(fd,0,SEEK_SET,0);
		printf("%s: second child release read lock\n",Gf_time());
		exit(0);
	}
	sleep(5);
	un_lock(fd,0,SEEK_SET,0);
	printf("%s: parent release read lock\n",Gf_time());
	exit(0);
	
}
