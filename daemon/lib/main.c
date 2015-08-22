#include "../include/socket_control.h"
#include "../include/configx.h"
#include "../include/pthreadxsx.h"
#include "../include/contain.h"
#include <unistd.h>

#include "../include/toolsfun.h"

int extid = 0;

int main(int argc, char const *argv[])
{
/*	if( daemon(1, 0) == -1)
		perror("daemon:");*/
	if(argc == 2)
	{
		extid = atoi(argv[1]);
		DEBUGW;
	}
	else if(argc == 1)
	{	
		char idchr[6];
		if(readcfg("id",idchr) == false)
		{
			DEBUGW;
			return 0;
		}
		DEBUGW;
		extid = atoi(idchr);
	}
/*	if(linkbash()==false)
		return 0;*/
	while(1)
		sleep(connect_host());
	DEBUGW;
	return 0;
}