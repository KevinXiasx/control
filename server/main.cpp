#include "socket_control.h"
//#include "configx.h"
#include "pthread_class.h"

int main(int argc, char const *argv[])
{
	Address addr;
	if(!addr.init("127.0.0.1",atoi(argv[1])))
		return 0;
	Socket sock;
	if(!sock.create_socket(atoi(argv[1])))
		return 0;
/*	if(!sock.listen_socket())
		return 0;
	int newfd = sock.accept_socket();
	if(newfd != -1)
	{
		void * buf = malloc(8);
		int sie = recv(newfd,buf,8,0);
		printf("%d date = %d\n",sie ,*((int*)buf));
	}*/
	int k = 9;
	if(sock.connect_socket(&addr))
	{
		cout<<sock.mysocketFd<<endl;
		int res= send(sock.mysocketFd,&k,4,0);
		cout<<"res ="<<res<<endl;
	}
	return 0;
}
