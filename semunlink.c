#include "unp.h"
int main(int argc,char **argv)
{
	if(argc!=2)
		err_quit("usage:semunlink <name>\n");
	if(sem_unlink(argv[1])<0)
		err_sys("sem_unlink");
	exit(0);
}
