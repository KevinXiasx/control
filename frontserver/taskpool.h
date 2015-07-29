#ifndef TASKPOOL_H_
#define TASKPOOL_H_ 

#include "packet_x.h"

class TaskPool
{
public:
	TaskPool();
	~TaskPool();

	int inserttask(const PacketClass *buf);//添加任务到任务池中,并返回任务池任务数量
	int gettask(PacketClass *res);//读取任务，并从任务池中删除该任务,并返回任务池任务数量,
	int gettask(PacketClass *res,int sec);//读取任务，并从任务池中删除该任务,并返回任务池任务数量,超时等待返回-1

private:
	std::list<PacketClass> taskpool;//任务池
	pthread_mutex_t taskLock;//任务池读写锁
	pthread_cond_t taskCond;//任务池条件变量
};


class TaskPoolS
{
public:
	TaskPoolS();
	~TaskPoolS();

	void inserttask(const PacketClass *buf);//添加任务到任务池中
	void gettask(PacketClass *res);//读取任务，并从任务池中删除该任务

	void sortingpacket();//分拣notsort中的任务
private:
	std::list<PacketClass> taskpool;//可取任务池
	pthread_mutex_t taskLock;//任务池读写锁
	pthread_cond_t taskCond;//任务池条件变量


	std::list<PacketClass> notsort;//待处理任务池
	pthread_mutex_t notsortLock;
	pthread_cond_t notsortCond;

	map< int, list<PacketClass> > taskmap;//正在受检的关联表
	pthread_mutex_t mapLock;
};


#endif

