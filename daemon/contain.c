#include "contain.h"
#include <unistd.h>
#include "config.h"
#include "socket_control.h"
#include "project_head.h"
#include "pthreadxsx.h"

extern int extid;

//==============================connect================================



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

