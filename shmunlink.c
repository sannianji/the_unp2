#include "unp.h"

int main(int argc,char **argv)
{
	if(argc!=2)
		err_quit("usage: shmunlink <name>");

	if(shm_unlink(argv[1])==-1)
		err_sys("shm_unlink");
	
	exit(0);
}
