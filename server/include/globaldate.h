#ifndef GLOBALDATE_H_
#define GLOBALDATE_H_ 
#include "../include/ioclass.h"
#include "../include/bdgmgerclass.h"
#include "../include/configx.h"
#include "../include/eventclass.h"
#include "../include/requestclass.h"
#include "../include/mysqlclass.h"
#include "../include/taskclass.h"


//此类为单例类，存放所有全局变量，以及互斥锁、条件变量等
class GlobalDate
{
public:
	static GlobalDate* create();
	~GlobalDate();

	bool check();

	IoClass *Io;
	BdgmgerClass *Bdgmger;
	EventClass *Event;
	Configx *Config;
	RequestClass *Request;
	MysqlClass *Mysql;


private:
	GlobalDate();
	GlobalDate(GlobalDate& me){}
	static GlobalDate* myglobalpr;
	bool mycheck;

};



#endif