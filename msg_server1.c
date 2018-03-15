#include "mesg.h"


void server(int readfd,int writefd)
{
	FILE* fp;
	char *ptr;
	pid_t pid;
	ssize_t n;
	struct mymesg mesg;

	for(;;)
	{
		mesg.mesg_type=1;
		if((n=mesg_recv(readfd,&mesg))==0)
		{
			err_msg("pathname missing");
			continue;
		}
		mesg.mesg_data[n]='\0';

		if((ptr=strchr(mesg.mesg_data,' '))==NULL)
		{
			err_msg("bogus request:%s\n",mesg.mesg_data);
			continue;
		}

		*ptr++=0;
		pid=atol(mesg.mesg_data);
		mesg.mesg_type=pid;


		if((fp=fopen(ptr,"r"))==NULL)
		{
			snprintf(mesg.mesg_data+n,sizeof(mesg.mesg_data)-n,"can't open,%s\n",strerror(errno));

			mesg.mesg_len=strlen(ptr);
			memmove(mesg.mesg_data,ptr,mesg.mesg_len);
			if(mesg_send(writefd,&mesg)<0)
				err_sys("mesg_send");
		}
		else
		{
			while(fgets(mesg.mesg_data,MAXMESGDATA,fp)!=NULL)
			{
				mesg.mesg_len=strlen(mesg.mesg_data);

				if(mesg_send(writefd,&mesg)<0)
					err_sys("mesg_send");
			}
			if(fclose(fp)<0)
				err_sys("close");
		}
	mesg.mesg_len=0;
	if(mesg_send(writefd,&mesg)<0)
		err_sys("mesg_send");
	}
}
