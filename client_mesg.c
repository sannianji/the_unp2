#include "unp.h"
#include "mesg.h"

void client(int readfd,int writefd)
{
	size_t len;
	ssize_t n;
	struct mymesg mesg;
	
	fgets(mesg.data,MAXMESGDATA,stdin);
	len=strlen(mesg.data);
	if(mesg.data[len-1]=='\n')
		len--;
	mesg.mesg_len=len;
	mesg.mesg_type=1;
	
	mesg_send(writefd,&mesg);
	
	while((n=mesg_recv(readfd,&mesg))>0)
		write(STDOUT_FILENO,mesg.mesg_data,n);
}
