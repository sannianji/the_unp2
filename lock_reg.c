#include "unp.h"

int lock_reg(int fd,int cmd,int type,off_t offset,int whence,off_t len)
{
	struct flock lock;
	lock.l_type=type;
	lock.l_start=offset;
	lock.l_whence=whence;
	lock.l_len=len;

	return (fcntl(fd,cmd,&lock));
}

pid_t lock_test(int fd,int type,off_t offset,int whence,off_t len)
{
	struct flock lock;
	lock.l_type=type;
	lock.l_start=offset;
	lock.l_whence=whence;
	lock.l_len=len;
	
	if(fcntl(fd,F_GETLK,&lock)==-1)
		return -1;
	if(lock.l_type==F_UNLCK)
		return 0;
	
	return lock.l_pid;
}
#define read_lock(fd,offset,whence,len) lock_reg(fd,F_SETLK,F_RDLCK,offset,whence,len)
#define readw_lock(fd,offset,whence,len) lock_reg(fd,F_SETLKW,F_RDLCK,offset,whence,len)
#define write_lock(fd,offset,whence,len) lock_reg(fd,F_SETLK,F_WRLCK,offset,whence,len)
#define write_lockw(fd,offset,whence,len) lock_reg(fd,F_SETLKW,F_WRLCK,offset,whence,len)
#define un_lock(fd,offset,whence,len) lock_reg(fd,F_SETLK,F_UNLCK,offset,whence,len)
#define is_read_lockable(fd,offset,whence,len) !lock_test(fd,F_RDLCK,offset,whence,len)
#define is_write_lockable(fd,offset,whence,len) !lock_test(fd,F_WRLCK,offset,whence,len)
