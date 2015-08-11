#ifndef ENCRYPT_H_
#define ENCRYPT_H_ 


int sendpt(int fd, const void *buf, unsigned size);

int recvpt(int fd, void *buf,unsigned size);

#endif