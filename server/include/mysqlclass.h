#ifndef MYSQLCLASS_H_
#define MYSQLCLASS_H_

#include <mysql.h>
#include <iostream>
#include <vector>
#include "../include/project_head.h"
#include <cstring>
#include <my_global.h>

using namespace std;

class MysqlClass
{
public:

	MysqlClass();
	~MysqlClass();

	bool connect_mysql();
	bool data_mysql(string sql);

	bool select_mysql(string sql,std::vector<string>* v);


private:
	
	MYSQL* sqlpr;
	pthread_mutex_t sqlmutex;
};

#endif
