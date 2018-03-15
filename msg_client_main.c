#include "svmsg.h"

void client(int ,int );


int main(int argc,char **argv)
{
	int readid,writeid;

	if((writeid=msgget(MQ_KEY1,0))<0)
		err_sys("msgget write");
	if((readid=msgget(MQ_KEY2,0))<0)
		err_sys("msgget read");


	client(readid,writeid);

	if((msgctl(readid,IPC_RMID,NULL))<0)
		err_sys("msgctl readid");
	if((msgctl(writeid,IPC_RMID,NULL))<0)
		err_sys("msgctl writeid");

	exit(0);
}
