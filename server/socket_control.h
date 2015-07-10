#ifndef SOCKET_CONTROL_H_
#define SOCKET_CONTROL_H_

#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/socket.h>
#include "project_head.h"

using namespace std;

class Address
{
public:
	Address(struct sockaddr_in* myaddr);
	bool init(string ip, int port);

	int port;
	string ip;
	struct sockaddr_in myaddr;		
	int socketfd;                    //be used in transmission between pthread and pthread
};


#define CONST_PORT 2223

class Socket
{
public:

	bool create_socket();
	bool bind_socket(int port);
	bool listen_socket(int max = SOMAXCONN);
	int accept_socket(Address * client_addr = NULL);
	bool connect_socket(const Address * addr);
	int mysocketFd;
private:
	
	
};

int connect_x(int socket,struct sockaddr_in * myAddr);
#endif /* SOCKET_CONTROL_H_ */
