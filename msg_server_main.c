#include "svmsg.h"
void server(int,int);

int main(int argc,char **argv)
{
	int readid,writeid;
	
	if((readid=msgget(MQ_KEY1,SVMSG_MODE|IPC_CREAT))<0)
		err_sys("msgget");
	if((writeid=msgget(MQ_KEY2,SVMSG_MODE|IPC_CREAT))<0)
		err_sys("msgget");

	server(readid,writeid);
	exit(0);
}
