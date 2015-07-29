#include "worker.h"
#include "project_head.h"
#include "pthreadclass"

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
		io->out("-- all --  choose all device \n-- show -- show all device\n-- ID -- choose one of devices\n-- q -- quit\n");
		string in1 = io->in();
		if(in1 == "all")
		{
			str_v.clear();
			data->select_mysql("select id from devices where heart='y'",&str_v);
			if(str_v.size() == 0)
			{
				io->out("hasn`t device !\n");
				return false;
			}
			else
			{
				char str[20] = {0};
				sprintf(str,"%s devises is avtive and  been choosed!\n",str_v.size());
				io->out(str);
				for (std::vector<string>::iterator i = str_v.begin(); i != str_v.end(); ++i)
					id_v->push_back(atoi(i->c_str()));
				return true;
			}
		}
		else if(in1 == "show")
		{
			str_v.clear();
			char st1[100];
			select_mysql("select id from devices where heart='y'",&str_v);
			for(int i=0;i < str_v.size(); i++)
			{
				sprintf(st1,"%-10s",v[i].c_str(),);
				io->out(st1);
				if(i%9 == 0 && i != 0)
					io->out("\n");
			}
			continue;
		}
		else if(atoi(in1.c_str()) != 0)
		{
			io->out("1 devise is avtive and  been choosed!\n");
			id_v->push_back(atoi(in1.c_str()));
			return true;
		}
		else if( in1 == "q")
		{
			return false;
		}
		else
		{
			io->out("illegal input\n");
			continue;
		}
	}
}


void sendshell_cb(int sock, short event, void* arg);
{
	GlobalDate* date = GlobalDate::create();
	TaskClass * shelltask = (TaskClass *)arg;
	int id = shelltask->mymsg.shell_m.id;

	int n = sendpt(sock,&(shelltask->mymsg),sizeof(U_MSG));
	if(n != sizeof(U_MSG))
	{
		if(n < 0 )
		{
			shelltask->failedid.push_back(id);
			date->Io.err("data send fail\n");
			return ;
		}
		else
		{
			shelltask->failedid.push_back(id);
			date->Io.err("date send imperfect\n");			
			return ;
		}
	}

	n = sendpt(sock,shelltask->mycommand.c_str(),shelltask->mycommand.size());
	if(n != shelltask->mycommand.size())
	{
		if(n < 0 )
		{
			shelltask->failedid.push_back(id);
			date->Io.err("data send fail\n");
			return ;
		}
		else
		{
			shelltask->failedid.push_back(id);
			date->Io.err("date send imperfect\n");			
			return ;
		}
	}

}

void reportshell_cb(int sock, short event, void* arg);
{
	GlobalDate* date = GlobalDate::create();
	TaskClass * shelltask = (TaskClass *)arg;
	int id = shelltask->mymsg.shell_m.id;

	U_MSG rep;
	int n = recvpt(sock,&rep,sizeof(U_MSG));
	if( n!=sizeof(U_MSG) )
	{
		if(n == 0)
		{
			date->Event.erasevent(sock);
			date->Sockmger.erasesock(sock);
			close(sock);
			shelltask->successfulid.push_back(id);
		}
		else
			shelltask->successfulid.push_back(id);
	}
	else if( rep.type == T_ANSWR)
	{
		if(rep.answer_m.torf == 1)
			shelltask->successfulid.push_back(id);
		else
			shelltask->failedid.push_back(id);
	}

	if( (shelltask->successfulid.size() + shelltask->failedid.size()) == shelltask->plannedid.size() )
	{
		
		pthread_exit(NULL);
	}
}


void* shellworker(void * argument)
{
	GlobalDate* date = GlobalDate::create();

	vector<int> id_v;
	if(!getdevice(&id_v))
		return;

	date->Io.out("please input shell command !\n");
	string command = date->Io.in();

	U_MSG taskmsg;
	taskmsg.shell_m.type = T_SHELL;
	taskmsg.shell_m.commandlen = command.size();

	TaskClass * shelltask = new TaskClass(&taskmsg, command, &id_v);

	EventClass repork;
	int tmpsock;
	for (std::vector<int>::iterator i = id_v.begin(); i != id_v.end(); ++i)
	{
		shelltask->mymsg.shell_m.id = *i;
		tmpsock = date->Sockmger.getsock(*i);
		date->Event.erasevent(tmpsock);
		repork.createwrite(tmpsock,sendhead_cb,shelltask);
		repork.createread(tmpsock,reportshell_cb,shelltask);
	}
	repork.run();
}

void tfile_worker()
{
	getdevice();
	newtask();

	sendtask()
	{
		remoteevent();
		neweventbase();

		sendhead();

		sendfile();
		waitreport();
	}
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
			pthread_t pthid = taskpthread.run(shellworker);
			pthread_join(pthid,NULL);
		}
		else
		{
			date->Io.out("choose illegal !\n");
		}
	}
}


//--------------------------communicate--------------------------
