#ifndef MANAGER_H_
#define MANAGER_H_ 

#define QUESTMAX 100
#define ANSWERMAX 2048
//----------------------------------------------------------1
#define AMENDCONFIG 1  
typedef struct admenconfig_r
{
	char config[50];
	char vauel[50];
}st_admenc_r;//修改运行时的配置信息,请求

typedef struct admenconfig_a
{
	bool issuccese;
}st_admenc_a;//修改运行时的配置信息回复
//-----------------------------------------------------------2
#define GETONLINE 2
typedef struct getonline_r
{
	
}st_getonline_r;//获取当前服务器运行的状态数据
typedef struct getonline_a	
{
	int onlinenumb;//有效连接数
	int onlineuser;//在线登录用户
	int respacket;//接受请求包的数量
	int senpacket;//发送回复包的数量
	int errpacket;
	int pthreadnumbDepacket;


}st_getonline_a;

//------------------------------------------------------
typedef struct M_request
{
	int type;
	char information[QUESTMAX];
}st_request;

typedef struct M_answer
{
	int type;
	char information[ANSWERMAX];
}st_answer;


#endif