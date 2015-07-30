#include "socket_control.h"
#include "config.h"
#include "pthreadxsx.h"
#include "contain.h"
#include <unistd.h>

int main(int argc, char const *argv[])
{

	if(linkbash()==false)
		return 0;
	int b = atoi(argv[1]);
	timer_connect_host(&b);

	//timer_connect_host(NULL);
	return 0;
}