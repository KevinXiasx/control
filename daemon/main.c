#include "socket_control.h"
#include "config.h"
#include "pthreadxsx.h"
#include "contain.h"
#include <unistd.h>
#include <event.h>


struct event_base* mybase = NULL;
int extid = 0;

int main(int argc, char const *argv[])
{
	extid = atoi(argv[1]);

	if(linkbash()==false)
		return 0;

	Timer(1,connect_host,NULL);

	event_base_dispatch(mybase);
	return 0;
}