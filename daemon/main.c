#include "socket_control.h"

int main(int argc, char const *argv[])
{
	if(argc<3)
	{
		printf("argument is too few\n");
		return 0;
	}
	address_x addr;
	address_init(&addr,argv[1],atoi(argv[2]));
	int sockfd = create_socket_x();
	if(sockfd == -1)
		return 0;
	if( connect_x(sockfd,&addr) == -1)
		return 0;
	else
		send(sockfd,&(addr.port),4,0);
	
/*	bind_x(sockfd,2223);
	listen_x(sockfd);
	int newfd = accept_x(sockfd,&addr);
*/
	return 0;
}