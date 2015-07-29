#include "socket_control.h"
#include "config.h"
#include "pthreadxsx.h"
#include "contain.h"
#include <unistd.h>

int main(int argc, char const *argv[])
{

	if(linkbash()==false)
		return 0;
	timer_connect_host(NULL);

	//timer_connect_host(NULL);
	return 0;
}