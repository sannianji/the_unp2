#include "mesg.h"
static err_quit(const char *str1,int n1,int n2)
{
	printf(str1,n1,n2);
	fflush(stdout);
	exit(-1);
}
ssize_t mesg_send(int fd,struct mymesg *mptr)
{
	return (write(fd,mptr,MESGHDRSIZE+mptr->mesg_len));
}

ssize_t mesg_recv(int fd,struct mymesg *mptr)
{
	size_t len;
	ssize_t n;
	
	if((n=read(fd,mptr,MESGHDRSIZE))==0)
		return 0;
	else if(n!=MESGHDRSIZE)
		err_quit("message header: expected %d,got %d",MESGHDRSIZE,n);

	if((len=mptr->mesg_len)>0)
		if((n=read(fd,mpte->mesg_data,len))!=len)
			err_quit("message data: expected %d,got %d",len,n);
	
	return len;
}
