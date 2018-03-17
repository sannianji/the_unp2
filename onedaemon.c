#include "unp.h"
#define PATH_PIDFILE "pidfile"

int main(int argc,char **argv)
{
	int pidfd;
	char line[MAXLINE];
	
	pidfd=open(PATH_PIDFILE,O_RDWR|O_CREAT,FILE_MODE);
}
