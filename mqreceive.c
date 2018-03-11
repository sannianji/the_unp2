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
	int c,flags;
	mqd_t mqd;
	ssize_t n;
	unsigned int prio;
	void *buff;
	struct mq_attr attr;
	flags=O_RDONLY;
	while((c=getopt(argc,argv,"n"))!=-1)
	{
		switch(c)
		{
			case 'n':
				flags|=O_NONBLOCK;
				break;
			case '?':
				exit(1);
		}
	}
	if(optind!=argc-1)
		err_quit("usage: mqreceive [-n] <name>\n");
	if((mqd=mq_open(argv[optind],flags))<0)
		err_sys("mq_open error: %s\n",strerror(errno));
	if(mq_getattr(mqd,&attr)<0)
		err_sys("mq_getattr error: %s\n",strerror(errno));
	if((buff=malloc(attr.mq_msgsize))==NULL)
		err_sys("malloc error: %s\n",strerror(errno));
	
	if((n=mq_receive(mqd,buff,attr.mq_msgsize,&prio))<0)
		err_sys("mq_reciev error: %s\n",strerror(errno));
	
	printf("read %ld bytes,priority=%u\n",(long)n,prio);

	exit(0);
}
