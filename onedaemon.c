#include "unp.h"
#define PATH_PIDFILE "pidfile"

int main(int argc,char **argv)
{
	int pidfd;
	char line[MAXLINE];
	
	pidfd=open(PATH_PIDFILE,O_RDWR|O_CREAT,FILE_MODE);
	if(write_lock(pidfd,0,SEEK_SET,0)<0)
	{
		if(errno==EACCES||errno==EAGAIN)
			err_quit("uable to lock %s, is %s already running?\n",PATH_PIDFILE,argv[0]);
		else
			err_sys("unable to lock %s\n",PATH_PIDFILE);
	}
	snprintf(line,sizeof(line),"%ld",(long)getpid());
	
	if(ftruncate(pidfd,0)<0)
		err_sys("ftruncate");
	if(write(pidfd,line,strlen(line))!=strlen(line))
		err_sys("write");

	pause();
}
