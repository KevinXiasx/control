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



//------------------------------------------create_socket()----------------------------------------------
//function:create a new socket
//argument: struct about address
//return : succese--(socket id),err--(-1)
//------------------------------------------------------------------------------------------------------------
int create_socket_x(int port)
{
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	int mysock = socket(PF_INET,SOCK_STREAM,0);
	ERR(mysock,-1,"socket create fail !",err_return);

	myaddr.sin_port = htons(port);
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int opt = 1;
	setsockopt(mysock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

/*	int res = bind(mysock,(struct sockaddr*)(&myaddr),sizeof(struct sockaddr_in));
	ERR(res,-1,"blind address is fail ! ",err_return);*/

	return mysock;
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


