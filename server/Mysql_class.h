#ifndef MYSQLCLASS
#define MYSQLCLASS 

#include <mysql.h>
#include <my_global.h>
#include <string.h>
#include "configx.h"

using namespace std;


class MysqlClass
{
public:
	MysqlClass();
	~MysqlClass();

	bool connect_mysql();
	bool data_mysql(string sql);

	// vector<string> select_mysql(string sql);
private:
	MYSQL* sqlpr;
};

#endif
