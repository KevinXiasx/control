#include "socket_control.h"

int main(int argc, char const *argv[])
{
	address_x addr;
	address_init(&addr,"127.0.0.1",2223);
	int sockfd = create_socket_x();
	if(sockfd == -1)
		return 0;
/*	if( connect_x(sockfd,&addr) == -1)
		return 0;
	else
	{
		printf("addr = %d\n",addr.port);
		send(sockfd,&(addr.port),4,0);
	}*/
	bind_x(sockfd,2223);
	listen_x(sockfd);
	printf("sdf\n");
	int newfd = accept_x(sockfd,&addr);
	if(newfd!=-1)
	{
		printf("sdafssss\n");
		void * buf = malloc(8);
		int sie = recv(newfd,buf,8,0);
		printf("%d date = %d\n",sie ,*((int*)buf));
	}
	return 0;
}