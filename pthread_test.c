#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
void thread1_exit(void *arg)
{
	write(STDOUT_FILENO,"exit from thread1\n",19);
}
void *thread1(void* arg)
{
	pthread_cleanup_push(thread1_exit,NULL);
	for(;;)
	{
		write(STDOUT_FILENO,"thread1 running\n",17);
	}
	pthread_cleanup_pop(1);
	return NULL;
}

int main()
{
	pthread_t tid_thread1;
	pthread_create(&tid_thread1,NULL,(void *)thread1,NULL);
	pthread_detach(tid_thread1);
	sleep(3);
	write(STDOUT_FILENO,"main thread exit\n",18);
	exit(0);
}
