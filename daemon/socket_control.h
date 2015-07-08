#ifndef SOCKET_CONTROL_H_
#define SOCKET_CONTROL_H_

#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/socket.h>
#include "project_head.h"

#define IP_LEN 15
typedef struct addres{
	int port;
	char ip[IP_LEN];
	struct sockaddr_in myaddr;
}address_x;

void address_init(address_x * addr);


int create_socket_x(address_x * myAddr);
int listen_x(int socket);
int accept_x(int socket,address_x * myAddr);
int connect_x(int socket,address_x * myAddr);


#endif /* SOCKET_CONTROL_H_ */
