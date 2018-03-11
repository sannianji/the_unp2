#include "unp.h"
struct mq_attr attr;
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
	int c,flags;
	mqd_t mqd;
	
	flags=O_RDWR|O_CREAT;

	
	while((c=getopt(argc,argv,"em:z:"))!=-1)
	{
		switch (c)
		{
			case 'e':
				flags|=O_EXCL;
				break;
			case 'm':
				attr.mq_maxmsg=atol(optarg);
				break;
			case 'z':
				attr.mq_msgsize=atol(optarg);
				break;
			case '?':
				exit(1);
		}
	}
	if(optind!=argc-1)
		err_quit("usage: mecreate [-e] [-m maxmsg -z msgsize] <name>\n");
	if((attr.mq_maxmsg!=0&&attr.mq_msgsize==0)||(attr.mq_maxmsg==0&&attr.mq_msgsize!=0))
		err_quit("must specify both -m maxmsg and -z msgsize\n");

	if((mqd=mq_open(argv[optind],flags,FILE_MODE,(attr.mq_maxmsg!=0)?&attr:NULL))<0)
		err_sys("mq_open error:%s\n",strerror(errno));
	
	mq_close(mqd);
	exit(0);
}
