#include "Mysql_class.h"

MysqlClass::MysqlClass()
{
	sqlpr = NULL;
}

MysqlClass::~MysqlClass()
{
	mysql_close(sqlpr);
}

bool MysqlClass::data_mysql(string sql)
{
	if(mysql_real_query(sqlpr, sql.c_str(),sql.size()+1) == NULL)
	{
		printf("Error %u: %s\n", mysql_errno(sqlpr), mysql_error(sqlpr));
		return false;
	}
	return true;
}

bool MysqlClass::connect_mysql()
{
	sqlpr = mysql_init(NULL);
	if (sqlpr == NULL) 
	{
		printf("line :%d Error %u: %s\n",__LINE__, mysql_errno(sqlpr), mysql_error(sqlpr));
		return false;
	}
 	while(mysql_real_connect(sqlpr, NULL, "root", "danjina", "androiad_divice", 0, NULL, 0) == NULL) 
	{
		printf("line :%d Error %u: %s\n",__LINE__, mysql_errno(sqlpr), mysql_error(sqlpr));
		if( mysql_errno(sqlpr) == 1049 )
		{
			if(mysql_real_connect(sqlpr, NULL, "root", "danjina", NULL, 0, NULL, 0) != NULL)
			{
				DEBUGW;
				if(data_mysql("create database androiad_divice"))
				{
					DEBUGW;
					continue;
				}
				else
				{
					DEBUGW;
					mysql_close(sqlpr);
					continue;
				}
			}
		}
		else
		{
			DEBUGW;
			return false;
		}
	}
	DEBUGW;
	return data_mysql("create table if not exists ipaddress(id int not null primary key,ip char(15) not null)");

}

vector<string> MysqlClass::select_mysql(string sql)
{
	vector<string> v;
	if( !data_mysql(sql) )
		return v;
	MYSQL_RES *res_set;
	res_set = mysql_store_result(sqlpr);
	MYSQL_ROW row;
    while ((row = mysql_fetch_row(res_set)) != NULL)
    {
        for(int i=0;i<mysql_num_fields(res_set);i++)
        {
            v.push_back(row[i]);
        }
    }
    if(mysql_errno(sqlpr) != 0)
    {
    	v.clear();
    	return v;
    }
	mysql_free_result(res_set);
	return v;
}