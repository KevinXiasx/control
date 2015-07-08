#include "socket_control.h"

bool Address::init(string ip, int port)
{
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(port);
	int res = inet_aton(ip.c_str(),&(myaddr.sin_addr));
	if(res == 0)
		return false;
	else
		return true;
}

bool Socket::create_socket(int port)
{
	mysocketFd = socket(PF_INET,SOCK_STREAM,0);
	ERR(mysocketFd,-1,"socket create fail !",false);

	struct sockaddr_in address_my;
	address_my.sin_family = AF_INET;
	address_my.sin_port = htons(port);
	address_my.sin_addr.s_addr = htonl(INADDR_ANY);
	int opt = 1;
	setsockopt(mysocketFd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

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
	socklen_t len = 0;
	if(client_addr == NULL)
		res = accept(mysocketFd,NULL,NULL);
	else
		res = accept(mysocketFd,(struct sockaddr*)(&(client_addr->myaddr)),&len);
	ERR(res,-1,"accept is fail ! ",err_return);
	return res;
}
bool Socket::connect_socket(Address * addr)
{
	int res = 0;
	res = connect(mysocketFd,(struct sockaddr*)(&(addr->myaddr)),sizeof(struct sockaddr_in));
	ERR(res,-1,"connect fail ! ",err_return);

	return true;
}

/*int create_socket_x(address_x * myAddr)
{
	int mysock = socket(PF_INET,SOCK_STREAM,0);
	ERR(mysock,-1,"socket create fail !",-1);

	int res = bind(mysock,(struct sockaddr*)(&(myAddr->myaddr)),sizeof(struct sockaddr_in));
	ERR(res,-1,"blind address is fail ! ",-1);

	return mysock;
}

int listen_socket(int socket)
{
	int res = listen(socket,SOMAXCONN);
	ERR(res,-1,"listen fail",err_return);
	return socket;
}

int accept_x(int socket,address_x * myAddr)
{
	int res = 0;
	int len = sizeof(struct sockaddr_in);
	res = accept(socket,(struct sockaddr*)(&(myAddr->myaddr)),&len);
	ERR(res,-1,"accept is fail ! ",err_return);
	return res;
}

int connect_x(int socket,address_x * myAddr)
{
	int res = 0;
	res = connect(socket,(struct sockaddr*)(&(myAddr->myaddr)),sizeof(struct sockaddr_in));
	ERR(res,-1,"connect fail ! ",err_return);

	return socket;
}

*/

/*void address_init(const void * input, void * output)
{
	printf("1\n");
	int res = 0;
	address_x * addr_input = (address_x * )input;
	addr_input->myaddr.sin_family = AF_INET;
	addr_input->myaddr.sin_port = htons(addr_input->port);
	res = inet_aton(addr_input->ip,&(addr_input->myaddr.sin_addr));
	if(res == 0)
	{
		perror("address trans fail ! ");
		*(bool_t *)output = false;
	}
	else
	*(bool_t *)output = true;
	printf("1.1\n");
}

void address_destroy(const void * input, void * output)
{
	printf("2\n");
	*(bool_t *)output = true;
}
void address_less(const void * input1, const void * input2,void * output)
{
	printf("3\n");
	*(bool_t *)output = true;
}
void address_copy(const void * input1, const void * input2,void * output)
{
	printf("4\n");

	address_x* a = (address_x*)input1;
	address_x* b = (address_x*)input2;

	printf("port out = %d\n",a->port);
	printf("port in = %d\n",b->port);

	memcpy(a,b,sizeof(address_x));
	printf("i'm here\n");
	*(bool_t *)output = true;
}*/


