
#include "../include/worker.h"
#include "../include/project_head.h"

//==================================kill============================


/*void killer(int sock, short event, void* arg)
{
	GlobalDate* date = GlobalDate::create();
	Bridge* bdg = NULL;
	std::vector<string> st_v;
	date->Mysql->select_mysql("select id from devices where breathe='n' and heart='y'",&st_v);
	for (vector<string>::iterator i = st_v.begin(); i != st_v.end(); ++i)
		date->Bdgmger->erasebdg(atoi(i->c_str()),KEY_ID,FLAG_DEL);

	date->Mysql->data_mysql("update devices set heart='n' where breathe='n'");

	date->Mysql->data_mysql("update devices set breathe='n'");

	free(arg);
	date->Event->createtimer(KILLTIME,killer);
}
*/
//----------------------------------kill-------------------------


//===========================requestworker===========================

void regist(const U_MSG* req,Bridge * bdg)
{
	GlobalDate* date = GlobalDate::create();
	int maxid;
	std::vector<string> v;

	if(!(date->Mysql->select_mysql("select max(id) from devices",&v)))
	{
		date->Io->err("max id err");
		return ;
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
		return ;
	else
	{
		char sql[300] = {0};
		sprintf(sql,"insert into devices(id,heart,breathe) values(%d,'y','y')",maxid);
		date->Mysql->data_mysql(sql);
		bdg->setid(maxid);
		date->Bdgmger->addbdg(bdg);
		cout<<"one has register \n";
	}
	return ;
}


void beatheart(const U_MSG* req, Bridge * bdg)
{
	GlobalDate* date = GlobalDate::create();

	time_t t= time(NULL);
	struct tm *mytime = localtime(&t);

	char sql[300] = {0};
	vector<string> str_v;
	sprintf(sql,"select * from devices where id=%d",req->heart_m.id);
	date->Mysql->select_mysql(sql,&str_v);
	if(str_v.size() == 0)
	{
		if(date->Bdgmger->erasebdg(bdg->socket(),KEY_SOCK,FLAG_DEL) == NOEXIST)
			delete bdg;
		return ;
	}

	sprintf(sql,"update devices set breathe='y',heart='y',breathetime='%02d%02d%02d%02d%02d' where id=%d",mytime->tm_year-100,mytime->tm_mon+1,mytime->tm_mday,mytime->tm_hour,mytime->tm_min,req->heart_m.id);
	date->Mysql->data_mysql(sql);
	cout<<"one has branthe \n";
	if(bdg->id() == Unknow)
	{
		bdg->setid(req->heart_m.id);
		if(date->Bdgmger->addbdg(bdg) == EXIST)
			delete bdg;
	}
}


//---------------------------requestworker---------------------------


//============================network============================
void read_cb(int sock, short event, void* arg)
{
	GlobalDate* date = GlobalDate::create();
	Bridge* bdg = (Bridge*)arg;
	U_MSG buf;
	int n = recvpt(bdg->socket(),&buf,sizeof(U_MSG));
	if(n <= 0)
	{
		if(date->Bdgmger->erasebdg(bdg->socket(),KEY_SOCK,FLAG_DEL) == NOEXIST)
			delete bdg;
	}
	else if( n == sizeof(U_MSG) )
	{
		switch(buf.type)
		{
			case T_HEART:
				beatheart(&buf,bdg);
				break;
			case T_REGST:
				regist(&buf,bdg);
				break;
			default:
				break;
		}
	}
}

void accpet_cb(int sock, short event, void* arg)
{
	int newfd;
	GlobalDate* date = GlobalDate::create();

	newfd = accept(sock, NULL, NULL);

	Bridge* bdg = new Bridge(newfd);
	bdg->intoevt(date->Event,read_cb,FLAG_READ,bdg);
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
	Bridge* bdg = new Bridge(sock.mysocketFd);
	bdg->intoevt(date->Event,accpet_cb,FLAG_READ);

	date->Event->run();
}

//---------------------------network---------------------------

