#include "socket_control.h"
#include "project_head.h"

int address_init(address_x * addr,const char * ips, int fport)
{
	addr->myaddr.sin_family = AF_INET;
	addr->port = fport;
	addr->myaddr.sin_port = htons(fport);
	memset(addr->ip,0,sizeof(addr->ip));
	memcpy(addr->ip,ips,strlen(ips));
	int res = inet_aton(addr->ip,&(addr->myaddr.sin_addr));
}



int create_socket_x()
{
	int mysock = socket(PF_INET,SOCK_STREAM,0);
	ERR(mysock,-1,"socket create fail !",err_return);

	int opt = 1;
	setsockopt(mysock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	return mysock;
}

int bind_x(int socket,int port)
{
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port);
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int res = bind(socket,(struct sockaddr*)(&myaddr),sizeof(struct sockaddr_in));
	ERR(res,-1,"blind address is fail ! ",err_return);
}

int listen_x(int socket)
{
	int res = listen(socket,SOMAXCONN);
	ERR(res,-1,"listen fail",err_return);
	return socket;
}

int accept_x(int socket,address_x * myAddr)
{
	int res = 0;
	socklen_t len = 0;
	if ( myAddr == NULL)
	{
		printf("herer\n");
		res = accept(socket,NULL,NULL);
	}
	else
	{
		printf("111herer\n");
		res = accept(socket,(struct sockaddr*)(&(myAddr->myaddr)),&len);
	}
	ERR(res,-1,"accept is fail ! ",err_return);
	return res;
}

int connect_x(int socket,address_x * myAddr)
{
	int res = 0;
	int i = 0;
	res = connect(socket,(struct sockaddr*)(&(myAddr->myaddr)),sizeof(struct sockaddr_in));
	ERR(res,-1,"connect fail ! ",err_return);

	return socket;
}
