#include "socket_control.h"

Address::Address()
{
	
}

void Address::rinit()
{
	port = ntohs(myaddr.sin_port);
	char iptem[INET_ADDRSTRLEN];
	ip = inet_ntop(AF_INET,&myaddr.sin_addr,iptem,INET_ADDRSTRLEN);
}

bool Address::init(string ip, int port)
{
	this->ip = ip;
	this->port = port;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port);
	int res = inet_pton(AF_INET,ip.c_str(),&(myaddr.sin_addr));
	ERR(res,0,"address_err",false);
	ERR(res,-1,"address_err",false);
	return true;
}

bool Socket::create_socket()
{
	mysocketFd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	ERR(mysocketFd,-1,"socket create fail !",false);

	int opt = 1;
	setsockopt(mysocketFd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	return true;
}

bool Socket::bind_socket(int port)
{
	struct sockaddr_in address_my;
	address_my.sin_family = AF_INET;
	address_my.sin_port = htons(port);
	address_my.sin_addr.s_addr = htonl(INADDR_ANY);

	int res = bind(mysocketFd,(struct sockaddr*)(&(address_my)),sizeof(struct sockaddr_in));
	ERR(res,-1,"blind address is fail ! ",false);
	return true;
}

bool Socket::listen_socket(int max)
{
	int res = listen(mysocketFd,max);
	ERR(res,-1,"listen fail",err_return);
	return true;
}
int Socket::accept_socket(Address * client_addr)
{
	int res = 0;
	socklen_t len = sizeof(struct sockaddr_in);
	if(client_addr == NULL)
		res = accept(mysocketFd,NULL,NULL);
	else
		res = accept(mysocketFd,(struct sockaddr*)(&(client_addr->myaddr)),&len);
	ERR(res,-1,"accept is fail ! ",err_return);
	return res;
}

bool Socket::connect_socket(const Address * addr)
{
	int res = 0;
	res = connect(mysocketFd,(struct sockaddr*)(&(addr->myaddr)),sizeof(struct sockaddr_in));
	ERR(res,-1,"connect fail ! ",err_return);

	return true;
}


int connect_x(int socket,struct sockaddr_in * myAddr)
{
	int res = 0;
	int i = 0;
	res = connect(socket,(struct sockaddr*)myAddr,sizeof(struct sockaddr_in));
	ERR(res,-1,"connect fail ! ",err_return);

	return socket;
}
