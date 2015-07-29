#include "xsystem.h"
#include "globalDate.h"
#include <netdb.h>
extern Configx myConfig;//配置类

int init_sockst()
{
	int mysocket = socket(PF_INET,SOCK_STREAM,0);
	struct sockaddr_in address_my;
	address_my.sin_family = AF_INET;
	address_my.sin_port = htons(myConfig.getconfint("port"));
	address_my.sin_addr.s_addr = htonl(INADDR_ANY);
	int opt = 1;
	setsockopt(mysocket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	bind(mysocket,(struct sockaddr*)&address_my,sizeof(address_my));


	listen(mysocket,SOMAXCONN);

	int flags; 
	flags = fcntl(mysocket, F_GETFL);
	fcntl(mysocket, F_SETFL, flags | O_NONBLOCK);

	int epollfd = epoll_create(EPOLL_MAX);

	struct epoll_event event;
	struct epoll_event events[EPOLL_EVENT_MAX];

	event.events = EPOLLIN|EPOLLET;
	event.data.fd = mysocket;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,mysocket,&event);

	bool while_1 = true;
	struct sockaddr_in address_connect;
	int packetSize = sizeof(st_packet);
	socklen_t len = 0;
	PacketClass k;
	GlobalDate* globaldate= GlobalDate::create();

	Pthread_x heartbeat;//心跳检测线程
	heartbeat.run(lifedetection,&epollfd);

	while(while_1)
	{
		cout<<"begin listen "<<endl;
		int res = epoll_wait(epollfd,events,EPOLL_EVENT_MAX,-1);
		if(res == 0)
		{
			cout<<"timeout"<<endl;
			continue;
		}
		for(int i = 0;i<res;i++)
		{
			if(events[i].data.fd == mysocket)
			{	
				int accpet_socket = accept(mysocket,(struct sockaddr*)&address_connect,&len);
				if(globaldate->monitordata.onlineNumb.dosome(RETURN) >= myConfig.getconfint("onlinenumbtop"))
				{
					close(accpet_socket);
				}
				else
				{
					event.data.fd = accpet_socket;
					int flags;
					flags = fcntl(accpet_socket, F_GETFL);
					fcntl(accpet_socket, F_SETFL, flags | O_NONBLOCK);
					epoll_ctl(epollfd,EPOLL_CTL_ADD,accpet_socket,&event);
					int tempid = globaldate->myPort.addport(accpet_socket);//生成临时id发送给客户端
					k.helpclient(tempid);
					k.setFd(accpet_socket);
					globaldate->taskConp.inserttask(&k);
					globaldate->monitordata.onlineNumb.dosome(ADD);
				}
			}
			else
			{	
				int resfd = events[i].data.fd;
				int res = read(resfd,k.mollocPacket(),packetSize);
				if(res == -1)
				{
					perror("read err from socket");
					continue;
				}
				//---A-------------------------------掉线处理--------------------------------------
				if(res == 0)//有客户端退出
				{
					cout<<"someone is quit"<<endl;
					k.destroyPacket();
					globaldate->myPort.eraseport(resfd,epollfd);
					continue;
				}
				//---A------------------------------------ -----------------------------------------
				k.setFd(resfd);
				switch( k.iscomplete() )
				{
					case 1:
						globaldate->taskConp.inserttask(&k);//加入完整任务池
						break;
					case 2:
						globaldate->taskError.inserttask(&k);//进入错误包任务池
						cout<<"get a err packet from port "<<resfd<<endl;
						break;
					default:
						break;
				}			
			}
		}
	}
}



void* lifedetection(void* argument)//定时检测心跳
{
	int k = *(int*)argument;
	GlobalDate* globaldate= GlobalDate::create();
	while(true)
	{
		globaldate->myPort.outdieclient(k);
		sleep(myConfig.getconfint("heartbeattime"));
	}
}



void* talkmanager(void* argument)
{
	string k;
	while(true)
	{
		cin>>k;
		if(k == "q")
			exit(0);
	}
}
void* printfmsg(void* argument)
{
	GlobalDate* globaldate= GlobalDate::create();
	while(true)
	{
		sleep(2);
		cout<<"onlineNumb = "<<globaldate->monitordata.onlineNumb.dosome(RETURN)<<endl;
		cout<<"onlineUser = "<<globaldate->monitordata.onlineUser.dosome(RETURN)<<endl;
		cout<<"resPacket = "<<globaldate->monitordata.resPacket.dosome(RETURN)<<endl;
		cout<<"errPacket = "<<globaldate->monitordata.errPacket.dosome(RETURN)<<endl;
		cout<<"sendPacket = "<<globaldate->monitordata.sendPacket.dosome(RETURN)<<endl;
		cout<<"pthreadnumbDepacket = "<<globaldate->monitordata.pthreadnumbDepacket.dosome(RETURN)<<endl;
	}
}

void* manager(void* argument)//和管理进程进行交互
{
	GlobalDate* globaldate= GlobalDate::create();
	st_request request;
	st_answer answer;
	int type;
	//bool break_getline = true;
	while(true)
	{
		globaldate->requestMem.shmread(&request);
		type = request.type;
		switch(type)
		{
			case AMENDCONFIG://修改配置
				((st_admenc_a*)answer.information)->issuccese = myConfig.admen( ((st_admenc_r*)request.information)->config,((st_admenc_r*)request.information)->vauel );
				answer.type = AMENDCONFIG;
				globaldate->answerMem.shmwrite(&answer);
				break;
			case GETONLINE://获取当前服务器运行状态数据
				onlinedata();
				break;
			default:
				break;
		}

	}
}

void* onlinedata()
{
	GlobalDate* globaldate= GlobalDate::create();
	st_answer a;
	st_getonline_a c;
	c.onlinenumb = globaldate->monitordata.onlineNumb.dosome(RETURN);
	c.onlineuser = globaldate->monitordata.onlineUser.dosome(RETURN);
	c.respacket = globaldate->monitordata.resPacket.dosome(RETURN);
	c.senpacket = globaldate->monitordata.sendPacket.dosome(RETURN);
	c.pthreadnumbDepacket = globaldate->monitordata.pthreadnumbDepacket.dosome(RETURN);
	c.errpacket = globaldate->monitordata.errPacket.dosome(RETURN);
	a.type = GETONLINE;
	memcpy(a.information,&c,sizeof(st_getonline_a));
	globaldate->answerMem.shmwrite(&a);
}