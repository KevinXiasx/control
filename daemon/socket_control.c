#define _GNU_SOURCE
#include "socket_control.h"
#include "project_head.h"
#include <netdb.h>



/*int address_init(address_x * addr,const char * ips, int fport)
{
	addr->myaddr.sin_family = AF_INET;
	addr->port = fport;
	addr->myaddr.sin_port = htons(fport);

	memset(addr->ip,0,sizeof(addr->ip));
	memcpy(addr->ip,ips,strlen(ips));

	struct gaicb *reqs[1];
	reqs[0] = malloc(sizeof(*reqs[0]));
	memset(reqs[0], 0, sizeof(*reqs[0]));

	DEBUGW;
    reqs[0]->ar_name = addr->ip;

    int ret = getaddrinfo_a(GAI_WAIT, reqs, 1, NULL);
    DEBUGW;
	if (ret != 0) {
	    fprintf(stderr, "getaddrinfo_a() failed: %s\n",
	        gai_strerror(ret));
	    exit(EXIT_FAILURE);
	}
	DEBUGW;
	memcpy(&(addr->myaddr),reqs[0]->ar_result->ai_addr,sizeof(struct sockaddr));
	addr->myaddr.sin_port = htons(fport);
	char host[100];
	struct addrinfo *res;
	res = reqs[0]->ar_result;
	getnameinfo(res->ai_addr, res->ai_addrlen,host, sizeof(host),NULL, 0, NI_NUMERICHOST);
	puts(host);
	free(reqs[0]);
	
}
*/
/*int address_init(address_x * addr,const char * ips, int fport)
{
	addr->myaddr.sin_family = AF_INET;
	addr->port = fport;
	addr->myaddr.sin_port = htons(fport);
	memset(addr->ip,0,sizeof(addr->ip));
	memcpy(addr->ip,ips,strlen(ips));
	int res = inet_aton(addr->ip,&(addr->myaddr.sin_addr));
}
*/

int address_init(address_x * addr,const char * ips, int fport)
{
	struct addrinfo* lists;

	int res = getaddrinfo(ips,NULL,NULL,&lists);
	DEBUGI(res);
	if(res != 0)
		perror("info");
	char host[100];
	getnameinfo(lists->ai_addr, lists->ai_addrlen,host, sizeof(host),NULL, 0, NI_NUMERICHOST);
	puts(host);
	memcpy(&(addr->myaddr),lists->ai_addr,sizeof(struct sockaddr));

	addr->myaddr.sin_family = AF_INET;
	addr->port = fport;
	addr->myaddr.sin_port = htons(fport);
	memset(addr->ip,0,sizeof(addr->ip));
	memcpy(addr->ip,ips,strlen(ips));
	//int s = inet_aton(addr->ip,&(addr->myaddr.sin_addr));
}


int create_socket_x()
{
	int mysock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	ERR(mysock,-1,"socket create fail !",err_return);

	int opt = 1;
	int res = setsockopt(mysock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	if(res == -1)
		perror("sockopt ");

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
		res = accept(socket,NULL,NULL);
	}
	else
	{
		res = accept(socket,(struct sockaddr*)(&(myAddr->myaddr)),&len);
	}
	ERR(res,-1,"accept is fail ! ",err_return);
	return res;
}

int connect_x(int socket,address_x * myAddr)
{
	int res = 0;
	DEBUGW;
	res = connect(socket,(struct sockaddr*)(&(myAddr->myaddr)),sizeof(struct sockaddr));
	ERR(res,-1,"connect fail ! ",err_return);
	DEBUGW;
	return socket;
}
