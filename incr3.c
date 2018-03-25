#include "unp.h"

struct shared{
	sem_t mutex;
	int count;
}shared;

int main(int argc,char **argv)
{
	int fd,i,nloop;
	struct shared *ptr;
	
	if(argc!=3)
		err_quit("usage : incr3 <pathname>")
}
