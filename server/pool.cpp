#include "pool.h"
#include "Mysql_class.h"
#include <vector>
#include "myio.h"

//======================================accpet============================
void beatheart(Address* addr)
{
	Myio* io = Myio::createio();
	MysqlClass* data = MysqlClass::createsql();
	if(data == NULL)
	{
		io->myerr("mysql can`t connect ,date save fail");
		return;
	}
	addr->rinit();
	char sql[300] = {0};
	sprintf(sql,"select * from ipaddress where ip='%s'",addr->ip.c_str());
	string sql_str = sql;
	DEBUGS(sql);
	std::vector<string> v = data->select_mysql(sql_str);
	if(v.size() == 0)
	{
		sprintf(sql,"insert into ipaddress(id,ip,heart) values(null,'%s','y')",addr->ip.c_str());
		DEBUGS(sql);
		data->data_mysql(sql);
	}
	else
	{
		sprintf(sql,"update ipaddress set heart='y' where ip='%s'",addr->ip.c_str());
		DEBUGS(sql);
		data->data_mysql(sql);
	}
}


int mainlist()
{
	Socket sock;
	if(!sock.create_socket())
		return 0;
	if(!sock.bind_socket(CONST_PORT))
		return 0;
	if(!sock.listen_socket())
		return 0;
	int newfd = 0;
	while(1)
	{
		Address * addrbuf = new Address;
		newfd = sock.accept_socket(addrbuf);
		DEBUGI(newfd);
		if(newfd == -1)
		{
			delete addrbuf;
			continue;
		}
		addrbuf->socketfd = newfd;
		beatheart(addrbuf);
		delete addrbuf;
		close(newfd);
	}
}
//----------------------------------accpet-------------------------


//==================================shell============================

bool getdevice(vector<string> *v)
{
	Myio* io = Myio::createio();
	MysqlClass* data = MysqlClass::createsql();
	if(data == NULL)
	{
		io->myerr("mysql can`t connect ,date select fail");
		return false;
	}
	io->myout("if choose all device,input 'all' ,else 'show' show all device,'q' quit\n");
	string in1 = io->myin();
	if(in1 == "all")
	{
		*v = data->select_mysql("select ip from ipaddress where heart='y'");
		if(v->size() == 0)
		{
			io->myout("hasn`t device !\n");
			return false;
		}
	}
	else if(in1 == "show")
	{
		data->show();
	}
	else if(atoi(in1.c_str()) != 0)
	{
		char st1[100] = {0};
		sprintf(st1,"select ip from ipaddress where id=%d",atoi(in1.c_str()));
		*v = data->select_mysql(st1);
		if(v->size() == 0)
		{
			io->myout("hasn`t device !\n");
			return false;
		}
	}
	else if( in1 == "q")
	{
		return false;
	}
	else
	{
		io->myout("illegal input\n");
		return false;
	}
	return true;
}


bool exeshell()
{
	std::vector<string> devices;
	if( !getdevice(&devices) )
		return false;

}




//-----------------------------------shell----------------------------------------

void* communicate(void * argument)
{
	Myio* io = Myio::createio();
	MysqlClass* data = MysqlClass::createsql();
	char st1[100] = {0};
	std::vector<string> v = data->select_mysql("select * form ipaddress where heart='y'");
	sprintf(st1,"online devices about %d\n",v.size());
	io->myout(st1);

	string she;
	while(1)
	{
		io->myout("-q-  Quit\n");
		io->myout("-1-  translate files\n");
		io->myout("-2-  shell \n");
		io->myout("Your Choose :");
		she = io->myin();
		if(she == "1")
		{

		}
		else if(she == "q")
		{
			exit(0);
		}
		else if(she == "2" )
		{
			exeshell();
		}
		else 
		{
			io->myout("choose illegal !\n");
		}
	}
}


