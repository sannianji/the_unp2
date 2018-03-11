#include "unp.h"
static void err_quit(const char *str)
{
	printf(str);
	fflush(stdout);
	exit(-1);
}
static void err_sys(const char *str1,const char *str2)
{
	printf(str1,str2);
	fflush(stdout);
	exit(-1);
}
int main(int argc,char **argv)
{
	if(argc!=2)
		err_quit("usage: mqunlink <name>\n");
	if(mq_unlink(argv[1])<0)
		err_sys("mq_unlink error:%s\n",strerror(errno));
	exit(0);
}
