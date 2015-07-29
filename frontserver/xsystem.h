#ifndef XSYSTEM_H_
#define XSYSTEM_H_ 

#include "packet_x.h"
#include "pthread_class.h"


#define EPOLL_MAX 1100
#define EPOLL_EVENT_MAX 100

int init_sockst();//初始化网络连接端口



void* depacket(void* argument);//解包线程执行函数
void* sendpacket(void* argument);//从后置服务器读取包体处理函数
void* lifedetection(void* argument);//定时检测心跳

//void* depacketComp(void* argument);//分包任务池线程函数
//void* depacketsort(void* argument);//在分包池中収捡的线程

int errpacket(const PacketClass* packet);//对错误包进行处理的函数

void* talkmanager(void* argument);//用于交互的线程(服务器本身用于测试)
void* printfmsg(void* argument);

//--------------------------------------------------------
void* manager(void* argument);//和管理进程进行交互

	void* onlinedata(/*void* argument*/);


//--------------------------------------------------------	

void* depacketerr(void* argument);//错误包线程



#endif