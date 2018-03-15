#include "mesg.h"
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
		{
			fprintf(stderr,"message header: expected %d,got %d",MESGHDRSIZE,(int)n);
			fflush(stderr);
			exit(-1);
		}

	if((len=mptr->mesg_len)>0)
		if((n=read(fd,mptr->mesg_data,len))!=len)
			{
				fprintf(stderr,"message data: expected %d,got %ld",len,(int)n);
				fflush(stderr);
				exit(-1);
			}
	
	return len;
}
