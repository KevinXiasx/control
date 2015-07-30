
#include "worker.h"
#include "project_head.h"



//===========================requestworker===========================

void * regist(const U_MSG* req,Bridge * bdg)
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

	int n = sendpt(bdg->socket(),&buf,sizeof(U_MSG));

	if(n != sizeof(U_MSG))
		return NULL;
	else
	{
		char sql[300] = {0};
		sprintf(sql,"insert into devices(id,heart,breathe) values(%d,'y','y')",maxid);
		date->Mysql.data_mysql(sql);
		bdg->set(maxid);
		date->Bdgmger->addbdg(bdg);
		cout<<"one has register \n";
	}
	return NULL;
}


void beatheart(const U_MSG* req, Bridge * bdg)
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

	bdg->setid(req->heart_m.id);
	date->Bdgmger->addbdg(bdg);
}


void* requestworker(void * argument)
{
	GlobalDate* date = GlobalDate::create();
	U_MSG * buf = (U_MSG *)malloc(sizeof(U_MSG));
	Bridge * bdg;
	while(true)
	{
		int sock = date->Request.getrequest(buf);
		switch(buf->type)
		{
			case T_HEART:
				beatheart(buf,bdg);
				break;
			case T_REGST:
				regist(buf,bdg);
			default:
				break;
		}
	}
}

//---------------------------requestworker---------------------------


//==================================kill============================


void killer(int sock, short event, void* arg)
{
	GlobalDate* date = GlobalDate::create();
	Bridge* bdg = NULL;
	std::vector<string> st_v;
	date->Mysql->select_mysql("select id from devices where breathe='n' and heart='y'",&st_v);
	for (vector<string>::iterator i = st_v.begin(); i != st_v.end(); ++i)
		date->Bdgmger->erasebdg(atoi(i->c_str()),KEY_ID);

	date->Mysql->data_mysql("update devices set heart='n' where breathe='n'");

	date->Mysql->data_mysql("update devices set breathe='n'");

	free(arg);
	date->Event->createtimer(KILLTIME,killer);
}

//----------------------------------kill-------------------------


//============================network============================
void read_cb(int sock, short event, void* arg)
{
	GlobalDate* date = GlobalDate::create();
	Bridge* bdg = date->Bdgmger->getbdg(sock, KEY_SOCK);

	U_MSG buf;
	int n = recvpt(bdg->socket(),&buf,sizeof(U_MSG));
	if(n == 0)
		delete bdg;
	else if(n == -1)
		date->Io->err("read err");
	else if( n == sizeof(U_MSG) )
		date->Request->insertrequest(&buf,bdg->socket());
}


void accpet_cb(int sock, short event, void* arg)
{
	int newfd;
	GlobalDate* date = GlobalDate::create();

	newfd = accept(sock, NULL, NULL);

	date->Event->createread(newfd,read_cb);

	DEBUGM("one has connect");
}


int networker()
{
	GlobalDate* date = GlobalDate::create();

	Socket sock;
	if(!sock.create_socket())
		return 0;
	if(!sock.bind_socket(date->Config->getconfint("ctosport")))
		return 0;
	if(!sock.listen_socket())
		return 0;
	int newfd = 0;

	Bridge* bdg = new Bridge(sock.mysocketFd);
	bdg->intoevt(date->Event,accpet_cb,FLAG_READ);

	date->Event->run();
}

//---------------------------network---------------------------

