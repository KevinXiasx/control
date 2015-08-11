#include "../include/aes.h"
#include "../include/encrypt.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int sendpt(int fd, const void *buf, unsigned size)
{
/*	int times = size/16;
	if(size%16 != 0)
		times += 1;

	int wsize = 0, ret = 0;

	unsigned char key[16];
    unsigned char inbuf[16];
    unsigned char outbuf[16];

    aes_context ctx;
	memset(key, 0, 16);
 	memcpy(key, "xsx", 3);
    aes_setkey_enc(&ctx, key, 128);
    

	for (int i = 0; i < times; ++i)
	{
		if( (i == times-1) && (size%16 != 0) )
		{
			memset(inbuf, 0, 16);
			memcpy(inbuf, buf+16*i, size%16);
		}
		else
			memcpy(inbuf, buf+16*i, 16);
		aes_crypt_ecb(&ctx, AES_ENCRYPT, inbuf, outbuf);
		ret = write(fd,outbuf,16);
		if(ret <= 0)
			return ret;
	}
	return size;*/

	return send(fd, buf, size, 0);
}



int recvpt(int fd, void *buf,unsigned size)
{
/*	int times = size/16;
	if(size%16 != 0)
		times += 1;

	int rsize = 0, ret = 0;

	unsigned char key[16];
    unsigned char inbuf[16];
    unsigned char outbuf[16];

    aes_context ctx;
    memset(key, 0, 16);
 	memcpy(key, "xsx", 3);
    aes_setkey_dec(&ctx, key, 128);

	for (int i = 0; i < times; ++i)
	{
		ret = read(fd, inbuf, 16);
		if(ret <= 0)
			return ret;

		aes_crypt_ecb(&ctx, AES_DECRYPT, inbuf, outbuf);

		if( (i == times-1) && (size%16 != 0) )
			memcpy(buf+16*i, outbuf, size%16);
		else
			memcpy(buf+16*i, outbuf, 16);
	}
	return size;*/
	return recv(fd, buf, size, 0);
}

int decpipe(int srcfd, int dstfd, unsigned size)
{
/*	int times = size/16;
	if(size%16 != 0)
		times += 1;

	int rsize = 0, ret = 0;

	unsigned char key[16];
    unsigned char inbuf[16];
    unsigned char outbuf[16] ={0};

    aes_context ctx;
    memset(key, 0, 16);
 	memcpy(key, "xsx", 3);
    aes_setkey_dec(&ctx, key, 128);

	for (int i = 0; i < times; ++i)
	{
		ret = read(srcfd, inbuf, 16);
		if(ret <= 0)
			return ret;

		aes_crypt_ecb(&ctx, AES_DECRYPT, inbuf, outbuf);
		if( (i == times-1) && (size%16 != 0) )
			ret = write(dstfd, outbuf, size%16);
		else
			ret = write(dstfd, outbuf, 16);
	}
	return size;	*/
}
