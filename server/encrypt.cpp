#include "encrypt.h"
#include <unistd.h>

int sendpt(int fd, const void *buf, unsigned size)
{
	return write(fd,buf,size);
}



int recvpt(int fd, void *buf,unsigned size)
{
	return read(fd,buf,size);
}