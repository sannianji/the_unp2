#include "unp.h"

static void err_sys(const char *str)
{
	fputs(str,stderr);
	fprintf(stderr," error: %s",strerror(errno));
	exit(-1);
}
int main(int argc,char *argv[])
{
	int msqid;
	struct msqid_ds info;
	struct msgbuf buf;
	
	if((msqid=msgget(IPC_PRIVATE,SVMSG_MODE|IPC_CREAT))<0)
		err_sys("msgget");
	

	buf.mtype=1;
	buf.mtext[0]=1;
	
	if(msgsnd(msqid,&buf,1,0)<0)
		err_sys("msgsend");

	if(msgctl(msqid,IPC_STAT,&info)<0)
		err_sys("msgctl");


	printf("read-write:%03o,cbytes=%lu,qnum=%lu,qbytes=%lu\n",info.msg_perm.mode&0777,(unsigned long)info.msg_cbytes,(unsigned long)info.msg_qnum,(unsigned long)info.msg_qbytes);

	system("ipcs -q");
	
	if(msgctl(msqid,IPC_RMID,NULL)<0)
		err_sys("msgctl");
	exit(0);
}
