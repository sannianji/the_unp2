#include "unp.h"

void client(int readfd,int writefd)
{
	size_t len;
	ssize_t n;
	char buff[MAXLINE];
	
	fgets(buff,MAXLINE,stdin);
	len=strlen(buff);
	if(buff[len-1]=='\n')
		len--;

	write(writefd,buff,len);
	
	while((n=read(readfd,buff,MAXLINE))>0)
		write(STDOUT_FILENO,buff,n);
}
