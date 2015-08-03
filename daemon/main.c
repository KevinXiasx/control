#include "socket_control.h"
#include "configx.h"
#include "pthreadxsx.h"
#include "contain.h"
#include <unistd.h>

#include "toolsfun.h"

struct event_base* mybase = NULL;
int extid = 0;

int main(int argc, char const *argv[])
{
	extid = atoi(argv[1]);

	if(linkbash()==false)
		return 0;

/*	Timer(1,connect_host,NULL);

	event_base_dispatch(mybase);*/

	while(1)
	{
		sleep(connect_host());
	}
	return 0;
}