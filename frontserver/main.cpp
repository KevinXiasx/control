#include "xsystem.h"
#include "globalDate.h"

Configx myConfig;//系统配置类

int main(int argc, char const *argv[])
{
	GlobalDate* g = GlobalDate::create();//全局数据类，单例

	Pthread_Pool writePthread; //单包任务线程池
	writePthread.run(depacket,NULL,myConfig.getconfint("pthreadunder"));

	Pthread_Pool managerPrhread;//管理进程交互
	managerPrhread.run(manager,NULL,10);

	Pthread_x errproducePthread;//错误包体处理
	errproducePthread.run(depacketerr);

	Pthread_Pool readPthread;//从后置服务器读取包体线程
	readPthread.run(sendpacket,NULL,10);

	Pthread_x talkPthread;//交互线程
	talkPthread.run(talkmanager);
/*
	Pthread_x pri;
	pri.run(printfmsg);*/
	
	init_sockst();//初始化网络端口,并开始监听(心跳监听线程在此生成)

	delete g;
	return 0;
}
