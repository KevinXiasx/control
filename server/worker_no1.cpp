
#include "worker.h"
#include "project_head.h"



//===========================requestworker===========================

void * regist(const U_MSG* req, int sock)
{
	GlobalDate* date = GlobalDate::create();
	int maxid;
	std::vector<string> v;

	if(!(date->Mysql.select_mysql("select max(id) from devices",&v)))
	{
		date->Io.err("max id err");
		return NULL;
	}

	if(v[0] == "NULL")
		maxid = 1;
	else
		maxid = atoi(v[0].c_str())+1;
	
	U_MSG buf;
	buf.regist_m.type = T_REGST;
	buf.regist_m.id = maxid;

	int n = sendpt(sock,&buf,sizeof(U_MSG));

	if(n != sizeof(U_MSG))
		return NULL;
	else
	{
		char sql[300] = {0};
		sprintf(sql,"insert into devices(id,heart,breathe) values(%d,'y','y')",maxid);
		date->Mysql.data_mysql(sql);
		date->Sockmger.addsock(maxid,sock);
		cout<<"one has register \n";
	}
	return NULL;
}


void beatheart(const U_MSG* req,int sock)
{
	GlobalDate* date = GlobalDate::create();

	time_t t= time(NULL);
	struct tm *mytime = localtime(&t);

	char sql[300] = {0};
	vector<string> str_v;
	sprintf(sql,"select * from devices where id=%d",req->heart_m.id);
	date->Mysql.select_mysql(sql,&str_v);
	if(str_v.size() == 0)
		return ;

	sprintf(sql,"update devices set breathe='y',heart='y',breathetime='%02d%02d%02d%02d%02d' where id=%d",mytime->tm_year-100,mytime->tm_mon+1,mytime->tm_mday,mytime->tm_hour,mytime->tm_min,req->heart_m.id);
	date->Mysql.data_mysql(sql);
	cout<<"one has branthe \n";

	date->Sockmger.addsock(req->heart_m.id,sock);
}


void* requestworker(void * argument)
{
	GlobalDate* date = GlobalDate::create();
	U_MSG * buf = (U_MSG *)malloc(sizeof(U_MSG));
	while(true)
	{
		int sock = date->Request.getrequest(buf);
		switch(buf->type)
		{
			case T_HEART:
				DEBUGW;DEBUGI(sock);
				DEBUGW;DEBUGI(buf->heart_m.id);
				beatheart(buf,sock);
				break;
			case T_REGST:
				regist(buf,sock);
			default:
				break;
		}
	}
}

//---------------------------requestworker---------------------------


//==================================kill============================


/*
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
*/

void killer(int sock, short event, void* arg)
{
	GlobalDate* date = GlobalDate::create();

	std::vector<string> st_v;
	date->Mysql.select_mysql("select id from devices where breathe='n' and heart='y'",&st_v);
	for (std::vector<string>::iterator i = st_v.begin(); i != st_v.end(); ++i)
	{
		date->Event.eraseread(atoi(i->c_str()));
		date->Sockmger.erasesock(atoi(i->c_str()));
		close(date->Sockmger.getsock(atoi(i->c_str())));
	}
	date->Mysql.data_mysql("update devices set heart='n' where breathe='n'");

	date->Mysql.data_mysql("update devices set breathe='n'");

	date->Event.createtimer(KILLTIME,killer);
}

//----------------------------------kill-------------------------


//============================network============================
void read_cb(int sock, short event, void* arg)
{
	GlobalDate* date = GlobalDate::create();
	U_MSG buf;
	int n = recv(sock,&buf,sizeof(U_MSG),0);
	if(n == 0)
	{
		date->Event.eraseread(sock);
		date->Sockmger.erasesock(sock);
		close(sock);
	}
	else if(n == -1)
		date->Io.err("read err");
	else
	{
		DEBUGW;
		date->Request.insertrequest(&buf,sock);
	}
}


void accpet_cb(int sock, short event, void* arg)
{
	int newfd;
	GlobalDate* date = GlobalDate::create();

	newfd = accept(sock, NULL, NULL);
	DEBUGW;DEBUGI(newfd);
	date->Event.createread(newfd,read_cb);

	DEBUGM("one has connect");
}


int networker()
{
	GlobalDate* date = GlobalDate::create();

	Socket sock;
	if(!sock.create_socket())
		return 0;
	if(!sock.bind_socket(date->Config.getconfint("ctosport")))
		return 0;
	if(!sock.listen_socket())
		return 0;
	int newfd = 0;

	date->Event.createread(sock.mysocketFd,accpet_cb);

	date->Event.run();
}

//---------------------------network---------------------------

