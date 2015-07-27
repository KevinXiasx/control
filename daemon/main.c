#include "socket_control.h"
#include "config.h"
#include "pthreadxsx.h"
#include "contain.h"
#include <unistd.h>

int main(int argc, char const *argv[])
{
	pthread_run(timer_connect_host,NULL);

	const_listen_host(NULL);

	//timer_connect_host(NULL);
	return 0;
}