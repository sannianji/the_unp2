#include "svmsg.h"
void client(int,int);

int main(int argc,char **argv)
{
	int msqid;
	if((msqid=msgget(MQ_KEY1,0)<0))
		err_sys("msgget");

	client(msqid,msqid);
	exit(0);
}
