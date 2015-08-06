#include "socket_control.h"
#include "configx.h"
#include "pthreadxsx.h"
#include "contain.h"
#include <unistd.h>

#include "toolsfun.h"

int extid = 0;

int main(int argc, char const *argv[])
{
/*	if( daemon(1, 0) == -1)
		perror("daemon:");*/

	char idchr[6];
	if(readcfg("id",idchr) == false)
		return 0;

	extid = atoi(idchr);

	if(linkbash()==false)
		return 0;
	while(1)
	{
		sleep(connect_host());
	}
	return 0;
}