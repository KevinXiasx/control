#include "worker.h"
#include "project_head.h"
#include "pthreadclass.h"


/*#define T_HEART 0
#define T_ANSWR 1
#define T_SHELL 2
#define T_TFILE 3*/


//========================shell==========================

bool getdevice(vector<int> *id_v)
{
	GlobalDate* date = GlobalDate::create();
	std::vector<string> str_v;
	while(1)
	{
		date->Io.out("-- all --  choose all device \n-- show -- show all device\n-- ID -- choose one of devices\n-- q -- quit\n");
		string in1 = date->Io.in();
		if(in1 == "all")
		{
			str_v.clear();
			date->Mysql.select_mysql("select id from devices where heart='y'",&str_v);
			if(str_v.size() == 0)
			{
				date->Io.out("hasn`t device !\n");
				return false;
			}
			else
			{
				char str[20] = {0};
				sprintf(str,"%d devises is avtive and  been choosed!\n",str_v.size());
				date->Io.out(str);
				for (std::vector<string>::iterator i = str_v.begin(); i != str_v.end(); ++i)
					id_v->push_back(atoi(i->c_str()));
				return true;
			}
		}
		else if(in1 == "show")
		{
			str_v.clear();
			char st1[100];
			date->Mysql.select_mysql("select id from devices where heart='y'",&str_v);
			for(int i=0;i < str_v.size(); i++)
			{
				sprintf(st1,"%-10s",str_v[i].c_str());
				date->Io.out(st1);
				if(i%9 == 0 && i != 0)
					date->Io.out("\n");
			}
			continue;
		}
		else if(atoi(in1.c_str()) != 0)
		{
			date->Io.out("1 devise is avtive and  been choosed!\n");
			id_v->push_back(atoi(in1.c_str()));
			return true;
		}
		else if( in1 == "q")
		{
			return false;
		}
		else
		{
			date->Io.out("illegal input\n");
			continue;
		}
	}
}


void sendshell_cb(int sock, short event, void* arg)
{
	DEBUGW;
	GlobalDate* date = GlobalDate::create();
	TaskClass * shelltask = (TaskClass *)arg;
	int id = shelltask->mymsg.shell_m.id;
	DEBUGW;DEBUGI(sock);
	int n = sendpt(sock,&(shelltask->mymsg),sizeof(U_MSG));
	DEBUGW;DEBUGI(n);
	if(n != sizeof(U_MSG))
	{
		if(n < 0 )
		{
			shelltask->failedid.push_back(id);
			perror("send head:");
			date->Io.err("data send fail\n");
			goto reportlabel;
		}
		else
		{
			shelltask->failedid.push_back(id);
			date->Io.err("date send imperfect\n");			
			goto reportlabel;
		}
	}
	
	n = sendpt(sock,shelltask->mycommand.c_str(),shelltask->mycommand.size());
	DEBUGW;DEBUGI(n);
	DEBUGI(shelltask->mycommand.size());
	if(n != shelltask->mycommand.size())
	{
		if(n < 0 )
		{
			shelltask->failedid.push_back(id);
			date->Io.err("data send fail\n");
			perror("send head:");
			goto reportlabel;
		}
		else
		{
			shelltask->failedid.push_back(id);
			date->Io.err("date send imperfect\n");			
			goto reportlabel;
		}
	}
	shelltask->mymanager->erasewrite(sock);
	return ;

reportlabel:
	DEBUGW;
	shelltask->mymanager->erasewrite(sock);
	DEBUGI(shelltask->successfulid.size());
	DEBUGI(shelltask->plannedid.size());
	DEBUGI(shelltask->failedid.size());
	if( (shelltask->successfulid.size() + shelltask->failedid.size()) == shelltask->plannedid.size() )
	{
		DEBUGW;
		char result[100] = {0};
		sprintf(result,"Task has done,%d devices success, %d devices fial and them id save in database",shelltask->successfulid.size(), shelltask->failedid.size());
		date->Io.out(result);
		pthread_exit(NULL);
	}
}

void reportshell_cb(int sock, short event, void* arg)
{
	DEBUGW;
	GlobalDate* date = GlobalDate::create();
	TaskClass * shelltask = (TaskClass *)arg;
	int id = shelltask->mymsg.shell_m.id;

	U_MSG rep;
	int n = recvpt(sock,&rep,sizeof(U_MSG));
	if( n!=sizeof(U_MSG) )
	{
		DEBUGW;DEBUGI(n);
		if(n == 0)
		{
			DEBUGW;
			shelltask->mymanager->eraseread(sock);
			DEBUGW;
			date->Sockmger.erasesock(sock);
			DEBUGW;
			close(sock);
			shelltask->successfulid.push_back(id);
		}
		else
			shelltask->successfulid.push_back(id);
	}
	else if( rep.type == T_ANSWR)
	{
		DEBUGW;
		if(rep.answer_m.torf == 1)
			shelltask->successfulid.push_back(id);
		else
			shelltask->failedid.push_back(id);
	}
	DEBUGW;
	if( (shelltask->successfulid.size() + shelltask->failedid.size()) == shelltask->plannedid.size() )
	{
		DEBUGW;
		char result[100] = {0};
		sprintf(result,"Task has done,%d devices success, %d devices fial and them id save in database\n",shelltask->successfulid.size(), shelltask->failedid.size());
		date->Io.out(result);
		pthread_exit(NULL);
	}
}


void* shellworker(void * argument)
{
	GlobalDate* date = GlobalDate::create();

	vector<int> id_v;
	if(!getdevice(&id_v))
		return NULL;

	date->Io.out("please input shell command !\n");
	string command = date->Io.in();
	DEBUGW;
	U_MSG taskmsg;
	taskmsg.shell_m.type = T_SHELL;
	taskmsg.shell_m.commandlen = command.size();

	EventClass* repork = new EventClass;
	TaskClass * shelltask = new TaskClass(&taskmsg, command, &id_v,repork);

	int tmpsock;
	for (std::vector<int>::iterator i = id_v.begin(); i != id_v.end(); ++i)
	{
		DEBUGW;DEBUGI(*i);
		shelltask->mymsg.shell_m.id = *i;
		tmpsock = date->Sockmger.getsock(*i);
		DEBUGW;DEBUGI(tmpsock);
		date->Event.eraseread(tmpsock);
		repork->createwrite(tmpsock,sendshell_cb,shelltask);
		//sendshell_cb(tmpsock, 1, shelltask);
		repork->createread(tmpsock,reportshell_cb,shelltask);
		DEBUGW;
	}
	DEBUGW;
	repork->run();
}


//--------------------------shell--------------------------


//========================communicate==========================

void* communicate(void * argument)
{
	GlobalDate* date = GlobalDate::create();

	char st1[100] = {0};
	vector<string> v;
	date->Mysql.select_mysql("select id from devices where heart='y'",&v);
	sprintf(st1,"online devices about %d\n",v.size() );
	date->Io.out(st1);

	string she;
	while(1)
	{
		Pthread_x taskpthread;
		date->Io.out("-q-  Quit\n");
		date->Io.out("-1-  translate files\n");
		date->Io.out("-2-  shell \n");
		date->Io.out("Your Choose :");
		she = date->Io.in();
		if(she == "1")
		{

		}
		else if(she == "q")
		{
			exit(0);
		}
		else if(she == "2" )
		{
			taskpthread.run(shellworker);
			pthread_t pthid = taskpthread.pthreadid();
			DEBUGM("waiting pthread !");
			pthread_join(pthid,NULL);
			DEBUGM("pthread ending");
		}
		else
		{
			date->Io.out("choose illegal !\n");
		}
	}
}


//--------------------------communicate--------------------------
