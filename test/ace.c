#include "aes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    unsigned char key[32];
    unsigned char inbuf[16];
    unsigned char outbuf[16];
    unsigned char my[16];

    memset(key, 0, 32);
 	memcpy(key, "xsx", 3);

 	memcpy(inbuf,"123456789123456",16);
 	printf("%s\n", inbuf);

    aes_context ctx;

    aes_setkey_enc(&ctx, key, 128);

    aes_crypt_ecb(&ctx, AES_ENCRYPT, inbuf, outbuf);
	int i = 0;
    for (; i < 16; ++i)
    {
    	printf("%c\n", *(outbuf+i));
    }

    aes_setkey_dec(&ctx, key, 128);
    aes_crypt_ecb(&ctx, AES_DECRYPT, outbuf, my);
    printf("%s\n", my);
	return 0;
}