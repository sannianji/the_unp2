#include "unp.h"
static void err_sys(const char *str1,const char *str2)
{
	printf(str1,str2);
	fflush(stdout);
	exit(-1);
}
static void err_quit(const char *str)
{
	fputs(str,stdout);
	fflush(stdout);
	exit(-1);
}
int main(int argc,char **argv)
{
	mqd_t mqd;
	void *ptr;
	size_t len;
	unsigned int prio;
	
	if(argc!=4)
		err_quit("usage: mqsend <nmame><#bytes><priority>\n");
	len=atoi(argv[2]);
	prio=atoi(argv[3]);
	
	if((mqd=mq_open(argv[1],O_WRONLY))<0)
		err_sys("mq_open error:%s\n",strerror(errno));
	if((ptr=calloc(len,sizeof(char)))==NULL)
		err_sys("calloc error:%s\n",strerror(errno));

	if(mq_send(mqd,ptr,len,prio)<0)
		err_sys("mq_send error:%s\n",strerror(errno));
	exit(0);
}
