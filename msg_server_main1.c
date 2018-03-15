#include "svmsg.h"
void server(int,int);


int main(int argc,char **argv)
{
	int msqid;
	if((msqid=msgget(MQ_KEY1,SVMSG_MODE|IPC_CREAT))<0)
		err_sys("msgget");

	server(msqid,msqid);

	exit(0);
}
