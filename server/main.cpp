#include "socket_control.h"
//#include "configx.h"
#include "pthread_class.h"

int main(int argc, char const *argv[])
{
	Address addr;
	addr.init("127.0.0.1",atoi(argv[1]));

	Socket sock;
	if(!sock.create_socket(atoi(argv[1])))
		return 0;
	if(!sock.listen_socket())
		return 0;
	sock.accept_socket();
	return 0;
}
