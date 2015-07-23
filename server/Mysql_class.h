#ifndef MYSQLCLASS_H_
#define MYSQLCLASS_H_

#include <mysql.h>
#include <iostream>
#include <vector>
#include "project_head.h"
#include <cstring>

#include <my_global.h>

using namespace std;

class MysqlClass
{
public:
	static MysqlClass* createsql();
	~MysqlClass();

	bool connect_mysql();
	bool data_mysql(string sql);

	vector<string> select_mysql(string sql);

	void show();

private:
	MysqlClass();
	MYSQL* sqlpr;
	static MysqlClass* myclass;

	static bool connect;
};

#endif
