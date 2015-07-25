#include "Mysql_class.h"
#include "myio.h"

bool MysqlClass::connect = true;
MysqlClass* MysqlClass::myclass = NULL;

MysqlClass* MysqlClass::createsql()
{
	if(myclass == NULL)
	{
		myclass = new MysqlClass;
		if(!connect)
			delete myclass;
	}
	return myclass;
}

MysqlClass::MysqlClass()
{
	sqlpr = NULL;
	connect = connect_mysql();
}

MysqlClass::~MysqlClass()
{
	mysql_close(sqlpr);
	myclass = NULL;
}

bool MysqlClass::data_mysql(string sql)
{
	if(mysql_real_query(sqlpr, sql.c_str(),sql.size()+1) == NULL)
	{
		if(mysql_errno(sqlpr) == 0 )
			return true;
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
	bool createbool = data_mysql("create table if not exists ipaddress(id int primary key AUTO_INCREMENT,ip char(15) not null,heart char(1) not null,breathe char(1) not null)");
	cout<<"create bool = "<<createbool<<endl;
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

vector<string> MysqlClass::select_mysql(string sql)
{
	vector<string> v;
	if( !data_mysql(sql) )
		return v;
	DEBUGW;
	MYSQL_RES *res_set;
	res_set = mysql_store_result(sqlpr);
	if(res_set == NULL)
		return v;
	MYSQL_ROW row;
	DEBUGW;
    while ((row = mysql_fetch_row(res_set)) != NULL)
    {
        for(int i=0;i<mysql_num_fields(res_set);i++)
        {
            v.push_back(row[i]);
        }
    }
    DEBUGW;
    if(mysql_errno(sqlpr) != 0)
    {
    	v.clear();
    	return v;
    }
	mysql_free_result(res_set);
	return v;
}


void MysqlClass::show()
{
	Myio* io = Myio::createio();
	char st1[100];
	std::vector<string> v = select_mysql("select * from ipaddress where heart='y'");
	for(int i=0;i < v.size(); i=i+3)
	{
		sprintf(st1,"%-10s%-20s\n",v[i].c_str(),v[i+1].c_str());
		io->myout(st1);
	}
}
