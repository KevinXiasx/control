#ifndef GLOBALDATE_H_
#define GLOBALDATE_H_ 
#include "taskpool.h"
#include "configx.h"
#include "portx.h"
#include "sharemem.h"
#include "pthread_class.h"
#include "xlog.h"
#include "monitordata.h"
#include "manager.h"


//此类为单例类，存放所有全局变量，以及互斥锁、条件变量等
class GlobalDate
{
public:
	static GlobalDate* create();
	~GlobalDate();

	TaskPool taskConp;//单包任务池

	TaskPool taskError;//无法识别的包任务池
	
	Portx myPort;	//用户传输端口
	
	ShareMem<st_packet> writeMem;//用于给后置服务器写包
	ShareMem<st_packet> readMem;//从后置服务器读包

	ShareMem<st_request> requestMem;//用于接收管理任务
	ShareMem<st_answer> answerMem;//用于答复管理任务

	Monitordata monitordata;//监控数据

	Xlog mylog;//日志类
	
private:
	GlobalDate();
	GlobalDate(GlobalDate& me){}
	static GlobalDate* myglobalpr;

};



#endif