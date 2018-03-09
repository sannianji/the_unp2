#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 1024
int main(int argc,char **argv)
{
	size_t n;
	char buff[MAXLINE],command[MAXLINE];
	FILE *fp;

	if(fgets(buff,MAXLINE,stdin)==NULL&&(ferror(stdin)))
	{
		printf("fgets error\n");
		fflush(stdout);
		exit(-1);
	}
	n=strlen(buff);
	if(buff[n-1]=='\n')
		n--;
	
	snprintf(command,sizeof(command),"cat %s",buff);
	fp=popen(command,"r");
	
	while(fgets(buff,MAXLINE,fp)!=NULL)
		fputs(buff,stdout);
	pclose(fp);
	exit(0);
}
