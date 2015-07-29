#ifndef PACKET_H_
#define PACKET_H_ 

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <time.h>


using namespace std;
//此头文件定义了关于packet的结构体相关定义及宏定义

//--------------------------------------------------------------------------------------------------

#define USER_NAME_LENG 21  //用户名最长字符数+1
#define USER_PSW_LEND 21  //用户密码最长字符数+1
#define HEAD_NUMBER 0x4040
#define TAIL_NUMBER 0x5050

								 //包体类型宏定义
//-----------------------------------------------------------------心跳包
#define PACKET_BREAKHEARD 0000   //心跳包,无数据

typedef struct hearts
{
	int tempid;
}st_heart_s;

//-----------------------------------------------------------------登陆包
#define PACKET_LOGIN 1001      	 //登陆包

typedef struct loginc
{
	char name[USER_NAME_LENG];
	char psw[USER_PSW_LEND];
	char VersionNo[10];
}st_login_c;

typedef struct logins
{
	bool yn;
	int id;
}st_login_s;

//----------------------------------------------------------------主菜单列表包
#define PACKET_MAINLIST 1002      	 //主菜单列表包

typedef struct mainlistc
{
	
}st_mainlist_c;

typedef struct mainlists
{
	int area;
	int style;
	int year;
}st_mainlist_s;

//-----------------------------------------------------------------视频列表包
#define PACKET_FLASHLIST 1003      	 //视频列表包
typedef struct flashlistc
{
	char channel[10];
	char area[10];
	char style[10];
	char year[10];
}st_flashlist_c;

typedef struct flashlists
{
	int sum;
}st_flashlist_s;
typedef struct flashlists_
{
	char flashname[20];
	char ipaddress[40];
	int times;
}st_flashlist_s_;

//-----------------------------------------------------------------历史浏览记录包
#define PACKET_RECORD 1004      	 //历史浏览记录包

typedef struct recordc
{
	
}st_record_c;

typedef struct records
{
	int sum;
}st_record_s;

//--------------------------------------------------------------------------


//包头结构体
typedef struct packet_head    
{
	int idSend;	//客户端ID
	int numbPacket;  //分包序号，0开始
    int sumPacket;   //分包总数
    size_t datasize; //有效数据大小
    unsigned int CRC; //CRC校验码
}st_packethead;

typedef struct packet_stat
{
    int packetType;   //包体类型
    int idTrans;	//包体流水号
}st_packetstat;


#define INFORMATIONMAX 1024  //包体数据段最长可能长度
#define INFORMATIONMAXCHAR 1028
typedef struct packet 
{
	int headInt;
	st_packethead packethead; //包头
    st_packetstat packetstat;
	char information[INFORMATIONMAXCHAR]; //主体数据,实际可用数据空间为INFORMATIONMAX，需4字节为包尾
}st_packet;


#ifndef PACKETCLASS_H_
#define PACKETCLASS_H_ 

class PacketClass
{
public:
	PacketClass();
	PacketClass(int type);
	~PacketClass();



//创建需要发送的包体时才会用到的函数
	void helpclient(int tempid);
	void setType(int type);
	void answerset(const PacketClass* from);
	void setanswerdata(void* data,int datalen);
//----------------------------------


//×××××××××××××××××××××××××只有收包时才会调用的成员函数

	int checkPacket(int i = 0);
//-------------下面两个函数为从网络接口、前后置进程间读数据使用，其他时候不能调用!--------
	st_packet* mollocPacket();//!!!!
	void destroyPacket();//!!!!
	bool isable;//用于检测该类是否已经申请空间

//××××××××××××××××××××××××××××××××××××××××××××××××××××××××××
	st_packet* getPackPr(int i = 0) const;
	int getId() const;
	int getType() const;
	int getFd() const;
	int getPacketSize(int i = 0) const;
	int getErro() const;
	int getTrans() const;
	
	int iscomplete(int i = 0) const;
	bool islastpart() const;
	void setFd(int fd);

	PacketClass operator+(const PacketClass &p) const;
	bool operator<(const PacketClass &p) const;
	int getPacketNumb() const;



private:
	std::vector<st_packet*> myPackets;

	int packetnumb;

	int myid_fd;

	int errp;


};
#endif //PACKETCLASS_H_ 

#endif //PACKET_H_
