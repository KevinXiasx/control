#include "taskpool.h"
#include "xsystem.h"
//#include "configx.h"


//extern Configx myConfig;//配置类

TaskPool::TaskPool()
{
	pthread_mutex_init(&taskLock,NULL);
	pthread_cond_init(&taskCond,NULL);
}

int TaskPool::inserttask(const PacketClass *buf)//添加任务到任务池中
{
	int numb;
	pthread_mutex_lock(&taskLock);
	taskpool.push_back(*buf);
	numb = taskpool.size();
	pthread_mutex_unlock(&taskLock);
	pthread_cond_signal(&taskCond);
	return numb;
}
int TaskPool::gettask(PacketClass *res,int sec)//读取任务，并从任务池中删除该任务,
{
	int numb;
	struct timespec realtime;
	clock_gettime(CLOCK_REALTIME,&realtime);
	realtime.tv_sec += sec;
	pthread_mutex_lock(&taskLock);
	while(taskpool.size() == 0)
		if( pthread_cond_timedwait(&taskCond,&taskLock,&realtime) == ETIMEDOUT )
		{
			pthread_mutex_unlock(&taskLock);
			return -1;
		}
	*res = taskpool.front();
	taskpool.erase(taskpool.begin());
	numb = taskpool.size();
	pthread_mutex_unlock(&taskLock);
	return numb;
}
int TaskPool::gettask(PacketClass *res)
{
	int numb;
	pthread_mutex_lock(&taskLock);
	while(taskpool.size() == 0)
		pthread_cond_wait(&taskCond,&taskLock);
	*res = taskpool.front();
	taskpool.erase(taskpool.begin());
	numb = taskpool.size();
	pthread_mutex_unlock(&taskLock);
	return numb;
}

TaskPool::~TaskPool()
{
	pthread_mutex_destroy(&taskLock);
	pthread_cond_destroy(&taskCond);
}


//----------------------------------------------------------------------------------------------------

TaskPoolS::TaskPoolS()
{
	pthread_mutex_init(&taskLock,NULL);
	pthread_cond_init(&taskCond,NULL);
	pthread_mutex_init(&notsortLock,NULL);
	pthread_cond_init(&notsortCond,NULL);
	pthread_mutex_init(&mapLock,NULL);
}
TaskPoolS::~TaskPoolS()
{
	pthread_mutex_destroy(&taskLock);
	pthread_mutex_destroy(&notsortLock);
	pthread_mutex_destroy(&mapLock);
	pthread_cond_destroy(&taskCond);	
	pthread_cond_destroy(&notsortCond);
}

void TaskPoolS::inserttask(const PacketClass *buf)//添加任务到任务池中
{
	pthread_mutex_lock(&notsortLock);
	notsort.push_back(*buf);
	pthread_mutex_unlock(&notsortLock);
	pthread_cond_signal(&notsortCond);
}
void TaskPoolS::gettask(PacketClass *res)//读取任务，并从任务池中删除该任务
{
	pthread_mutex_lock(&taskLock);
	while(taskpool.size() == 0)
		pthread_cond_wait(&taskCond,&taskLock);
	*res = taskpool.front();
	taskpool.erase(taskpool.begin());
	pthread_mutex_unlock(&taskLock);
}

void TaskPoolS::sortingpacket()//分拣notsort中的任务
{
	PacketClass k;
	int temptrans;
	pthread_mutex_lock(&notsortLock);
	while(notsort.size() == 0)
		pthread_cond_wait(&notsortCond,&notsortLock);
	k = notsort.front();
	notsort.erase(notsort.begin());
	pthread_mutex_unlock(&notsortLock);
	int err = k.checkPacket();
	if(err != 0)
	{
		if(errpacket(&k) != 5)
			return;
	}
	temptrans = k.getTrans();
	map< int, list<PacketClass> >::iterator pr;
	pthread_mutex_lock(&mapLock);
	pr = taskmap.find(temptrans);
	if( pr == taskmap.end() )//该表中尚无该流水任务
	{
		std::list<PacketClass> v;
		v.push_back(k);
		taskmap.insert( pair<int, list<PacketClass> >(temptrans,v));
	}
	else
	{
		pr->second.push_back(k);
		pr->second.sort();
		if( pr->second.back().islastpart() )
		{
			PacketClass temp;
			for (std::list<PacketClass>::iterator i = pr->second.begin(); i != pr->second.end(); ++i)
			{
				temp = temp + *i;
			}
			pthread_mutex_lock(&taskLock);
			taskpool.push_back(temp);
			pthread_mutex_unlock(&taskLock);
			pthread_cond_signal(&taskCond);
			taskmap.erase(pr);
		}
	}
	pthread_mutex_unlock(&mapLock);
}

