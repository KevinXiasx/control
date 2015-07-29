#include "mysqlclass.h"
#include "ioclass.h"

MysqlClass::MysqlClass()
{
	sqlpr = NULL;
	pthread_mutex_init(&sqlmutex,NULL);
}

MysqlClass::~MysqlClass()
{
	mysql_close(sqlpr);
	pthread_mutex_destroy(&sqlmutex);
}

bool MysqlClass::data_mysql(string sql)
{
	bool re = false;
	cout<<"sql:"<<sql<<endl;
	pthread_mutex_lock(&sqlmutex);
	if(mysql_real_query(sqlpr, sql.c_str(),sql.size()+1) != 0)
	{
		if(mysql_errno(sqlpr) == 0 )
			re = true;
		else
			printf("Error %u: %s\n", mysql_errno(sqlpr), mysql_error(sqlpr));
	}
	else
	{
		re = true;
	}
	pthread_mutex_unlock(&sqlmutex);
	return re;
}

bool MysqlClass::connect_mysql()
{
	sqlpr = mysql_init(NULL);
	if (sqlpr == NULL) 
	{
		printf("line :%d Error %u: %s\n",__LINE__, mysql_errno(sqlpr), mysql_error(sqlpr));
		return false;
	}
 	while(mysql_real_connect(sqlpr, NULL, "root", "danjina", "android_device", 0, NULL, 0) == NULL) 
	{
		printf("line :%d Error %u: %s\n",__LINE__, mysql_errno(sqlpr), mysql_error(sqlpr));
		if( mysql_errno(sqlpr) == 1049 )
		{
			if(mysql_real_connect(sqlpr, NULL, "root", "danjina", NULL, 0, NULL, 0) != NULL)
			{
				if(data_mysql("create database android_device"))
				{
					continue;
				}
				else
				{
					mysql_close(sqlpr);
					continue;
				}
			}
		}
		else
		{
			return false;
		}
	}
	DEBUGW;
	bool createbool = data_mysql("create table if not exists devices(id int primary key AUTO_INCREMENT,heart char(1) not null,breathe char(1) not null,breathetime char(15))");
	return createbool;

/*	if(mysql_real_connect(sqlpr, NULL, "root", "danjina", NULL, 0, NULL, 0) != NULL) 
	{
		if(data_mysql("create database if not exists android_device"))
			if(data_mysql("create table if not exists ipaddress(id int primary key AUTO_INCREMENT,ip char(15) not null,heart char(1) not null,breathe char(1) not null)"))
				return true;
	}
	printf("line :%d Error %u: %s\n",__LINE__, mysql_errno(sqlpr), mysql_error(sqlpr));
	return false;*/
}

bool MysqlClass::select_mysql(string sql,vector<string>* v)
{
	if( !data_mysql(sql) )
		return false;
	MYSQL_RES *res_set;
	res_set = mysql_store_result(sqlpr);
	if(res_set == NULL)
		return true;
	MYSQL_ROW row;
    while ((row = mysql_fetch_row(res_set)) != NULL)
    {
        for(int i=0;i<mysql_num_fields(res_set);i++)
        {
        	if(row[i] == NULL)
        		v->push_back("NULL");
        	else
            	v->push_back(row[i]);
        }
    }
    if(mysql_errno(sqlpr) != 0)
    {
    	v->clear();
    	return false;
    }
	mysql_free_result(res_set);
	return true;
}


/*void MysqlClass::show()
{
	Myio* io = Myio::createio();
	char st1[100];
	std::vector<string> v = select_mysql("select id,ip from ipaddress where heart='y'");
	for(int i=0;i < v.size(); i=i+2)
	{
		sprintf(st1,"%-10s%-20s\n",v[i].c_str(),v[i+1].c_str());
		io->out(st1);
	}
}
*/