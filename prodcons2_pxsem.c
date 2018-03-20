#include "unp.h"
#define NBUFF 10

int nitems;
struct {
	int buff[NBUFF];
	sem_t mutex,nempty,nstored;
}shared;

void *produce(void *),*consume(void*);

int main(int argc,char **argv)
{
	pthread_t tid_produce,tid_consume;
	
	if(argc!=2)
		err_quit("usage: prodcons2 <#items>")
	nitems=atoi(argv[1]);

	if(sem_init(&shared.mutex,0,1)==-1)
		err_sys("shared.mutex sem_init");
	if(sem_init(&shared.nempty,0,NBUFF)<0)
		err_sys("shared.nempty sem_init");
	if(sem_init(&shared.nstored,0,1)==-1)
		err_sys("shared.nstored sem_init");

}
