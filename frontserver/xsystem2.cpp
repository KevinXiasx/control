#include "xsystem.h"
#include "globalDate.h"

extern Configx myConfig;//配置类

/*void* depacketsort(void* argument)//在分包池中収捡的线程
{
	GlobalDate* globaldate= GlobalDate::create();
	while(true)
	{
		globaldate->taskPart.sortingpacket();
	}
}

void* depacketComp(void* argument)//分包任务池线程函数
{
	GlobalDate* globaldate= GlobalDate::create();
	PacketClass k;
	while(true)
	{
		globaldate->taskPart.gettask(&k);//获取任务
		globaldate->log.addlog(&k,RES);
		switch( k.getType() )
		{
			case PACKET_BREAKHEARD://心跳处理
				globaldate->myPort.shockheart(&k);
				cout<<"pthread:"<<(int)pthread_self()<<"get heart "<<endl;
				globaldate->writeMem.shmwrite(k.getPackPr());
				break;
			case PACKET_LOGIN://登录包
				globaldate->writeMem.shmwrite(k.getPackPr());
				break;
			default://不识别的包体
				break;
		}
		k.destroyPacket();
	}
}
*/

void* depacket(void* argument)//单包解包线程执行函数
{
	GlobalDate* globaldate= GlobalDate::create();
	globaldate->monitordata.pthreadnumbDepacket.dosome(ADD);//统计执行任务的线程数量
	PacketClass k;
	int restask;
	while(true)
	{
		restask = globaldate->taskConp.gettask(&k,myConfig.getconfint("pthreadwaittime"));
		if(restask == -1)//表示等待超时了
		{
			if( globaldate->monitordata.pthreadnumbDepacket.dosome(RETURN) <= myConfig.getconfint("pthreadunder") )
				//当前线程数量小于或等于线程控制下限
			{
				continue;//则继续等待
			}
			else
			{
				globaldate->monitordata.pthreadnumbDepacket.dosome(DEL);
				break;//跳出循环，结束线程
			}
		}

		if( ( restask / globaldate->monitordata.pthreadnumbDepacket.dosome(RETURN) ) > myConfig.getconfint("pthreadpress")) 
			//若当前线程数量任务比低于配置要求，则生成下限数量的线程
		{
			Pthread_Pool writePthread; //单包任务线程池
			writePthread.run(depacket,NULL,myConfig.getconfint("pthreadunder"));
		}

		int err = k.checkPacket();//对包体内容完成性检验，返回错误码，0为无错误
		if(err != 0)//如果有错误
		{
			if(errpacket(&k) != 5)//如果不是粘包错误
				continue;
		}

		globaldate->mylog.addLog(&k,RES);//写入日志文件
		globaldate->myPort.shockheart(&k);//起搏心跳		

		switch( k.getType() )
		{
			case PACKET_BREAKHEARD://心跳处理
				globaldate->monitordata.resPacket.dosome(ADD);//统计收到的正确任务数量
				break;
			case PACKET_MAINLIST://此包是自己生成
				globaldate->writeMem.shmwrite(k.getPackPr());
				break;
			default://除了心跳包，其他丢给后置
				globaldate->writeMem.shmwrite(k.getPackPr());
				globaldate->monitordata.resPacket.dosome(ADD);//统计收到的正确任务数量
				break;
		}
		k.destroyPacket();
	}
}


void* sendpacket(void* argument)//从后置服务器读取包体处理函数
{
	GlobalDate* globaldate= GlobalDate::create();
	PacketClass k;
	k.mollocPacket();
	while(true)
	{
		globaldate->readMem.shmread(k.getPackPr());
		cout<<"get a packet from afterserver"<<endl;
		int sendfd = globaldate->myPort.getFd(k.getId());
		if(sendfd == -1)
		{
			cout<<"ID:"<<k.getId()<<" is Illegal logging !! Request has been ignored"<<endl;
			continue;
		}
		ssize_t si = write(sendfd,k.getPackPr(),k.getPacketSize());
		if( si != k.getPacketSize() )
		{
			cout<<"send to client fail !!"<<endl;
			globaldate->mylog.addLog(&k,ERR);
			continue;
		}
		globaldate->monitordata.sendPacket.dosome(ADD);
		cout<<"write a packet to client"<<endl;
		switch( k.getType() )
		{
			case PACKET_LOGIN://登陆包
				{
					st_login_s* cdata =(st_login_s* )(k.getPackPr()->information);
					if(cdata->yn)
					{
						globaldate->myPort.changeid(k.getId(),cdata->id);
						globaldate->monitordata.onlineUser.dosome(ADD);
					}
				}
			case PACKET_MAINLIST:
				globaldate->monitordata.sendPacket.dosome(DEL);
				break;
			default:
				break;
		}
		globaldate->mylog.addLog(&k,SEND);
	}
	k.destroyPacket();
}

void* depacketerr(void* argument)//错误包线程
{
	GlobalDate* globaldate= GlobalDate::create();
	PacketClass k;
	while(true)
	{
		globaldate->taskError.gettask(&k);//获取任务
		globaldate->mylog.addLog(&k,ERR);//写入错误日志
		globaldate->monitordata.errPacket.dosome(ADD);//统计数量

		k.destroyPacket();
	}
}

int errpacket(const PacketClass* k)//对错误包进行处理的函数
{
	GlobalDate* globaldate= GlobalDate::create();
	int errornumb = k->getErro();
	switch(errornumb)
	{
		case 5://粘包错误
			{
				PacketClass M;
				M.mollocPacket();
				memcpy(M.getPackPr(),(void*)((int)(k->getPackPr())+k->getPacketSize()),sizeof(st_packet)-k->getPacketSize());
				M.setFd(k->getFd());
				switch( M.iscomplete() )
				{
					case 1:
						globaldate->taskConp.inserttask(&M);
						break;
					case 2:
						globaldate->taskError.inserttask(&M);
						cout<<"get a err packet from port "<<k->getFd()<<endl;
						break;
					default:
						break;
				}
			}
			break;
		default:
			globaldate->taskError.inserttask(k);//直接进入错误包任务池
			break;
	}
	return errornumb;
}
