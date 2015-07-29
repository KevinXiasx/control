/*#include "configx.h"
#include "pool.h"
#include "Mysql_class.h"
#include <vector>
#include "myio.h"
#include <unistd.h>
#include "socket_control.h"



Configx Config;

//====================================accpet============================


void beatheart(Address* addr)
{
	Myio* io = Myio::createio();
	MysqlClass* data = MysqlClass::createsql();
	if(data == NULL)
	{
		io->err("mysql can`t connect ,date save fail");
		return;
	}
	addr->rinit();
	char sql[300] = {0};
	sprintf(sql,"select * from ipaddress where ip='%s'",addr->ip.c_str());
	string sql_str = sql;
	std::vector<string> v = data->select_mysql(sql_str);
	if(v.size() == 0)
	{
		sprintf(sql,"insert into ipaddress(id,ip,heart,breathe) values(null,'%s','y','y')",addr->ip.c_str());
		data->data_mysql(sql);
	}
	else
	{
		sprintf(sql,"update ipaddress set breathe='y',heart='y' where ip='%s'",addr->ip.c_str());
		data->data_mysql(sql);
	}
}


int mainlist()
{
	Socket sock;
	if(!sock.create_socket())
		return 0;
	if(!sock.bind_socket(Config.getconfint("ctosport")))
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


//==================================kill============================




bool killer(string ip)
{
	MysqlClass* data = MysqlClass::createsql();
	if(data == NULL)
	{
		return false;
	}
	char sql[300] = {0};
	sprintf(sql,"update ipaddress set heart='n',breathe='n' where ip='%s'",ip.c_str());
	return data->data_mysql(sql);
}


void* timer_kill_zombie(void* argument)
{
	sleep(1);
	MysqlClass* data = MysqlClass::createsql();
	if(data != NULL)
	{
		char sql[300] = {0};
		sprintf(sql,"update ipaddress set heart='n' where breathe='n'");
		data->data_mysql(sql);
		sprintf(sql,"update ipaddress set breathe='n'");
		data->data_mysql(sql);
	}
	sleep(3600*6);

}


//----------------------------------kill-------------------------



//==================================shell============================

bool getdevice(vector<string> *v)
{
	Myio* io = Myio::createio();
	MysqlClass* data = MysqlClass::createsql();
	if(data == NULL)
	{
		io->err("mysql can`t connect ,date select fail");
		return false;
	}
	while(1)
	{
		io->out("-- all --  choose all device \n-- show -- show all device\n-- ID -- choose one of devices\n-- q -- quit\n");
		string in1 = io->in();
		if(in1 == "all")
		{
			*v = data->select_mysql("select ip from ipaddress where heart='y'");
			if(v->size() == 0)
			{
				io->out("hasn`t device !\n");
				return false;
			}
		}
		else if(in1 == "show")
		{
			data->show();
			continue;
		}
		else if(atoi(in1.c_str()) != 0)
		{
			char st1[100] = {0};
			sprintf(st1,"select ip from ipaddress where id=%d",atoi(in1.c_str()));
			*v = data->select_mysql(st1);
			if(v->size() == 0)
			{
				io->out("hasn`t device !\n");
				return false;
			}
		}
		else if( in1 == "q")
		{
			return false;
		}
		else
		{
			io->out("illegal input\n");
			return false;
		}
		return true;
	}
}



int socketconnect(const vector<string> *devices,vector<Address> *online,vector<string> * offline)
{

	for(int i = 0; i< devices->size(); i++)
	{
		Address addr;
		if(!addr.init((*devices)[i],Config.getconfint("stocport")))
		{
			offline->push_back((*devices)[i]);
			continue;
		}
		Socket sock;
		if(!sock.create_socket())
		{
			offline->push_back((*devices)[i]);
			continue;
		}
		if(!sock.connect_socket(&addr))
		{
			offline->push_back((*devices)[i]);
			continue;
		}
		addr.socketfd = sock.mysocketFd ;
		online->push_back(addr);
	}
	return online->size();
}


int socketclose(vector<Address>* online)
{
	for (std::vector<Address>::iterator i = online->begin(); i != online->end(); ++i)
	{
		close(i->socketfd);
	}
}


int sendmsg(vector<Address>* online, const void* buff, int size)
{
	int cr = 0;
	for (std::vector<Address>::iterator i = online->begin(); i != online->end(); ++i)
	{
		int seek = write(i->socketfd,buff,size);
		if(seek == size)
			cr++;
		else
		{
			close(i->socketfd);
			i = online->erase(i);
			i--;
		}
	}
	return cr;
}



bool stdstep(vector<string>* offline,vector<Address>* online,U_MSG* msgbuff)
{
	Myio* io = Myio::createio();
	MysqlClass* data = MysqlClass::createsql();
	if(data == NULL)
	{
		io->err("mysql can`t connect");
		return false;
	}
	std::vector<string> devices;
	DEBUGW;
	if( !getdevice(&devices) )
		return false;

	int con_num = socketconnect(&devices,online,offline);
	if(con_num == 0)
	{
		io->out("sorry, has`n devices what can be connected!\n ");
		return false;
	}
	con_num = sendmsg(online,msgbuff,sizeof(U_MSG));
	if(con_num == 0)
	{
		io->out("sorry, has`n devices what can be connected! \n");
		return false;
	}
	return true;
}


bool exeshell()
{
	Myio* io = Myio::createio();

	std::vector<string> offline;
	std::vector<Address> online;

	U_MSG shell_buff;
	shell_buff.shell_m.id = ID_SHELL;

	if(!stdstep(&offline,&online,&shell_buff))
		return false;

	char st1[100];
	sprintf(st1,"%d devices has connect , please input shell :",online.size());
	io->out(st1);

	string shell = io->in();

	int con_num = sendmsg(&online,shell.c_str(),shell.size());
	sprintf(st1,"%d msg has sended\n",con_num);

	io->out(st1);

	socketclose(&online);
	return true;
}


//-----------------------------------shell----------------------------------------

//===================================file=========================================

bool filetrans(int fd,char* path)
{
	int n;
	char buf[4096];
	int filefd = open(path,O_RDONLY);
	ERR(filefd,-1,"file open fail",false);
	while( n = read(filefd,buf,4096) > 0 )
		if (n != write(fd,buf,4096) )
			perror("write err");
	if(n < 0)
		perror();
}

int tfile()
{

}



//-----------------------------------file-----------------------------------------


void* communicate(void * argument)
{
	Myio* io = Myio::createio();
	MysqlClass* data = MysqlClass::createsql();
	if(data == NULL)
	{
		io->err("mysql can`t connect\n");
		exit(1);
	}
	char st1[100] = {0};
	std::vector<string> v = data->select_mysql("select id from ipaddress where heart='y'");
	sprintf(st1,"online devices about %d\n",v.size());
	io->out(st1);

	string she;
	while(1)
	{
		io->out("-q-  Quit\n");
		io->out("-1-  translate files\n");
		io->out("-2-  shell \n");
		io->out("Your Choose :");
		she = io->in();
		if(she == "1")
		{
			tfile();
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
			io->out("choose illegal !\n");
		}
	}
}
*/