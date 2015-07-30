#include "globaldate.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

GlobalDate* GlobalDate::myglobalpr = NULL;

GlobalDate* GlobalDate::create()
{
	if(myglobalpr == NULL)
		myglobalpr = new GlobalDate;
	return myglobalpr;
}

GlobalDate::GlobalDate()
{
	Io = new IoClass;
	Bdgmger = new BdgmgerClass;
	Event = new EventClass;
	Config = new Configx;
	Request = new RequestClass;
	Mysql = new MysqlClass;
	
	if( !(mycheck = Mysql->connect_mysql()) )
		Io.out("database err, program can`t work \n");
}

GlobalDate::~GlobalDate()
{
	
}

bool GlobalDate::check()
{
	return mycheck;
}