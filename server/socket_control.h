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

	bool init(string ip, int port);

	int port;
	string ip;
	struct sockaddr_in myaddr;
};

class Socket
{
public:

	bool create_socket(int port);
	bool listen_socket(int max = SOMAXCONN);
	int accept_socket(Address * client_addr = NULL);
	bool connect_socket(Address * addr);
	int mysocketFd;
private:
	
	
};


#endif /* SOCKET_CONTROL_H_ */
