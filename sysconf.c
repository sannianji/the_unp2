#include <unistd.h>
#include <stdio.h>
int main()
{
	printf("%ld",sysconf(POSIX_IPC_PREFIX));
} 
