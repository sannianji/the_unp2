#include "mesg.h"
void client(int readfd,int writefd)
{
	size_t len;
	ssize_t n;
	char *ptr;
	struct mymesg mesg;

	snprintf(mesg.mesg_data,MAXMESGDATA,"%ld",(long)getpid());

	len=strlen(mesg.mesg_data);
	
	if((fgets(ptr,MAXMESGDATA-len,stdin)==NULL)&&(ferror(stdin)))
		err_sys("fgets");

	len=strlen(mesg.mesg_data);

	if(mesg.mesg_data[len-1]=='\n')
		len--;

	mesg.mesg_len=len;
	mesg.mesg_type=1;
	
	if(mesg_send(writefd,&mesg)<0)
		err_sys("mesg_send");


	mesg.mesg_type=getpid();

	while((n=mesg_recv(readfd,&mesg))>0)
		if(write(STDOUT_FILENO,mesg.mesg_data,n)!=n)
			err_sys("write");
}
