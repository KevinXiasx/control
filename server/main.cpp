#include "socket_control.h"
#include "configx.h"
#include "pthread_class.h"
#include "Mysql_class.h"

int main(int argc, char const *argv[])
{
	MysqlClass mysql;
	mysql.connect_mysql();
	return 0;








/*	Address addr;
	if(!addr.init(argv[1],atoi(argv[2])))
		return 0;
	Socket sock;
	if(!sock.create_socket())
		return 0;
	if(!sock.bind_socket(2223))
		return 0;
	if(!sock.listen_socket())
		return 0;
	int newfd = sock.accept_socket();
	if(newfd != -1)
	{
		void * buf = malloc(8);
		int sie = recv(newfd,buf,8,0);
		printf("%d date = %d\n",sie ,*((int*)buf));
	}
	int k = 9;
	if(sock.connect_socket(&addr))
	{
		cout<<sock.mysocketFd<<endl;
		int res= send(sock.mysocketFd,&k,4,0);
		cout<<"res ="<<res<<endl;
	}*/
	return 0;
}
