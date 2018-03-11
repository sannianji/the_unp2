#include "unp.h"
static void err_quit(const char *str1)
{
	printf(str1);
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
	mqd_t mqd;
	struct mq_attr attr;
	if(argc!=2)
		err_quit("usage: mqgetattr <name>\n");

	if((mqd=mq_open(argv[1],O_RDONLY))<0)
		err_sys("mq_open error :%s\n",strerror(errno));
	if(mq_getattr(mqd,&attr)<0)
		err_sys("mq_getattr error: %s\n",strerror(errno));

	printf("max $msgs=%ld,max #byts/msg=%ld, #currented on queue=%ld\n",attr.mq_maxmsg,attr.mq_msgsize,attr.mq_curmsgs);
	mq_close(mqd);
	exit(0);
	
}
