#include "../include/worker.h"
#include "../include/project_head.h"
#include "../include/pthreadclass.h"
 #include <sys/sendfile.h>

//========================shell==========================


void tasktimeout(int sock, short event, void* arg)
{
	static int wrisize = 0;
	GlobalDate* date = GlobalDate::create();
	TaskClass* task = (TaskClass*)arg;
	if( task->myManager()->getwrsize() == wrisize)
	{
		for (int i = 0; i < task->myDbg()->size(); ++i)
		{
			Bridge* bdg = (*(task->myDbg()))[i] ;
			if( bdg->myWrite() == NULL )
			{
				date->Bdgmger->erasebdg(bdg->socket(),KEY_SOCK,FLAG_NODEL);
				bdg->close();
				task->failbdg(bdg);
				bdg->outevt(FLAG_WRITE);
			}
		}
	}
	wrisize = task->myManager()->getwrsize();

	task->myManager()->createtimer(5, tasktimeout);
}



int selectnumb(vector<int>* dst,const string& src)
{
	string nextstring = src;
	while( true )
	{
		string::size_type index = nextstring.find(',');
		string tmp = nextstring.substr(0, index);
		
		string::size_type range;
		if( (range = tmp.find('-')) != string::npos)
		{
			int beginint = atoi( tmp.substr(0, range).c_str() );
			int endint = atoi( tmp.substr(range+1).c_str() );
			for (int i = beginint; i <= endint; ++i)
				dst->push_back(i);
		}
		else
		{
			int newid = atoi(tmp.c_str());
			if(newid != 0)
				dst->push_back(newid);
		}

		if(index == string::npos)
			break;
		else
			nextstring = nextstring.substr(index+1);
	}
}


bool getdevice(vector<Bridge*> *bdg_v)
{
	GlobalDate* date = GlobalDate::create();
	
	while(1)
	{
		date->Io->out("-- all --  choose all device \n-- show -- show all device\n-- ID -- choose one of devices\n-- q -- quit\n");
		string in1 = date->Io->in();
		if(in1 == "all")
		{
			if(date->Bdgmger->livebdg(bdg_v) == 0)
			{
				date->Io->out("hasn`t device !\n");
				return false;
			}
			else
			{
				printf("%d device is active and been choosed\n", bdg_v->size());
				return true;
			}
		}
		else if(in1 == "show")
		{
			vector<Bridge*> tmp_bdg;
			int numb =  date->Bdgmger->livebdg(&tmp_bdg);
			for(int i= 0; i< numb; i++)
			{
				char ids[20];
				sprintf(ids,"id : %d    ",tmp_bdg[i]->id());
				date->Io->out(ids);
			}
			continue;
		}
		else if(atoi(in1.c_str()) != 0)
		{
			vector<int> chooserange;
			selectnumb(&chooserange, in1);

			for (vector<int>::iterator i = chooserange.begin(); i != chooserange.end(); ++i)
			{
				Bridge* t = date->Bdgmger->getbdg(*i,KEY_ID);
				if( t == NULL )
					continue;
				else
				{
					printf("%-5d\n", *i);
					bdg_v->push_back(t);
				}
			}

			if( bdg_v->size() == 0)
			{
				date->Io->out("this id hasn`t device !\n");
				return false;
			}
			
			date->Io->out("These id have choosed, sure? \n-- y -- to continue\n-- n -- to choose again\n");
			string confime;
			cin>>confime;
			if(confime == "y" || confime == "Y")
				return true;
			else if(confime == "n" || confime == "N")
			{
				bdg_v->clear();
				continue;
			}
			else
			{
				bdg_v->clear();
				date->Io->out("illegal input\n");
				continue;
			}
		}
		else if( in1 == "q")
		{
			return false;
		}
		else
		{
			date->Io->out("illegal input\n");
			continue;
		}
	}
}


void sendshell_cb(int sock, short event, void* arg)
{
	GlobalDate* date = GlobalDate::create();
	Pak * pak = (Pak *)arg;
	int n = sendpt(sock,pak->prt,sizeof(U_MSG));
	n = sendpt(sock,pak->prt+sizeof(U_MSG),pak->size-sizeof(U_MSG));
	if(n != pak->size-sizeof(U_MSG))
	{
		date->Bdgmger->erasebdg(sock,KEY_SOCK,FLAG_NODEL);
		pak->bdg->close();
		if(n < 0 )
		{
			pak->task->failbdg(pak->bdg);
			perror("send head:");
			date->Io->err("data send fail\n");
			goto reportlabel;
		}
		else
		{
			pak->task->failbdg(pak->bdg);
			date->Io->err("date send imperfect\n");			
			goto reportlabel;
		}
	}
	pak->bdg->outevt(FLAG_WRITE);
	delete pak;
	return ;
reportlabel:
	if( pak->task->over() )
	{
		char result[100] = {0};
		sprintf(result,"Task has done,%d devices success, %d devices fial and them id save in database",pak->task->succsnum(),pak->task->failnum());
		date->Io->out(result);
		pak->task->ending(date->Event);
		delete pak->task;
		delete pak;
		pthread_exit(NULL);
	}
	else
	{
		delete pak;
	}
}

void reportshell_cb(int sock, short event, void* arg)
{
	GlobalDate* date = GlobalDate::create();
	TaskClass* task = (TaskClass*)arg;
	Bridge* bdg = date->Bdgmger->getbdg(sock,KEY_SOCK);
	char times[50];
	char Buf[2048] = {0};
	int n = recvpt(sock,Buf,2048);
	DEBUGI(bdg->id());DEBUGI(n);
	if( n<=0 )
	{
		date->Bdgmger->erasebdg(sock,KEY_SOCK,FLAG_NODEL);
		bdg->close();
		task->failbdg(bdg);
	}
	else if(n>0 && n<4)
	{
		return;
	}
	else
	{
		bool end = false;
		if( *((int*)(Buf+n-4)) == ENDINT )
			end = true;
		char filename[20] = {0};
		sprintf(filename, "log/log%d", bdg->id());

		int logfilefd = open(filename, O_CREAT|O_APPEND|O_RDWR, 0644);
		if( logfilefd == -1)
			return ;
		time_t t= time(NULL);
		struct tm *mytime = localtime(&t);
		sprintf(times, "[ TIME: %d-%d-%d, %d:%d:%d ]", mytime->tm_year-100,mytime->tm_mon+1,mytime->tm_mday,mytime->tm_hour,mytime->tm_min,mytime->tm_sec);
		write( logfilefd, times, strlen(times));
		write(logfilefd, Buf, end?n-4:n);
		if(end)
			write(logfilefd, "\n----------\n",11);
		close(logfilefd);
		if(end)
			task->succsbdg(bdg);
	}

	if(task->over() )
	{
		char result[100] = {0};
		sprintf(result,"Task has done,%d devices success, %d devices fial and them id save in database",task->succsnum(),task->failnum());
		date->Io->out(result);
		task->ending(date->Event);
		delete task;
		pthread_exit(NULL);
	}
}


void* shellworker(void * argument)
{
	GlobalDate* date = GlobalDate::create();

	vector<Bridge*> bdg_v;
	if(!getdevice(&bdg_v))
	{
		*(bool*)argument = false;
		return NULL;
	}

	date->Io->out("please input shell command !\n");
	string command = date->Io->in();

	*(bool*)argument = false;

	EventClass* repork = new EventClass;
	TaskClass * shelltask = new TaskClass(T_SHELL, command, &bdg_v,repork);
	
	shelltask->setwrbk(sendshell_cb);
	shelltask->setrdbk(reportshell_cb);
	shelltask->rgstask();
	tasktimeout(0, 0, shelltask);
	repork->run();
}


//--------------------------shell--------------------------


//==========================file===========================

void tfile_cb(int sock, short event, void* arg)
{
	GlobalDate* date = GlobalDate::create();
	TaskClass* task = (TaskClass*)arg;
	Bridge* bdg = date->Bdgmger->getbdg(sock,KEY_SOCK);
	U_MSG rep;

	int n = recvpt(sock,&rep,sizeof(U_MSG));
	if( n!=sizeof(U_MSG) )
	{
		perror("scok:");
		date->Bdgmger->erasebdg(sock,KEY_SOCK,FLAG_NODEL);
		bdg->close();
		task->failbdg(bdg);
	}
	else if( rep.type == T_ANSWR)
	{
		if(rep.answer_m.torf == 1)
		{
			string filepath;
			task->srcstring(filepath);
			int fd = open(filepath.c_str(),O_RDONLY);
			if(fd == -1)
			{
				task->failbdg(bdg);
				goto tfilereportlabel;
			}

			struct stat statbuf;
			if(fstat(fd, &statbuf)== -1)
			{
				task->failbdg(bdg);
				goto tfilereportlabel;				
			}
			int n = 0, m = 0;
			char filebuf[4096]  = {0};

			struct timeval timeout;
			timeout.tv_sec = 2;
			timeout.tv_usec= 0;
			socklen_t len = sizeof(timeout);
			setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout.tv_sec, len);

			while( (n=read(fd, filebuf, 4096)) > 0)
				if( (m=sendpt(sock, filebuf, n)) <= 0 )
				{
					break;
				}
			if(m > 0)
				task->succsbdg(bdg);
			else{
				date->Bdgmger->erasebdg(sock,KEY_SOCK,FLAG_NODEL);
				task->failbdg(bdg);
				bdg->close();
			}
		}
		else
			task->failbdg(bdg);
	}
tfilereportlabel:
	if( task->over() )
	{
		char result[100] = {0};
		sprintf(result,"Task has done,%d devices success, %d devices fial and them id save in database",task->succsnum(),task->failnum());
		date->Io->out(result);
		task->ending(date->Event);
		delete task;
		pthread_exit(NULL);
	}
}


void* tfileworker(void* argument)
{
	GlobalDate* date = GlobalDate::create();

	vector<Bridge*> bdg_v;
	if(!getdevice(&bdg_v))
	{
		*(bool*)argument = false;
		return NULL;
	}
	while(1)
	{
		date->Io->out("please input \"/sourse-path to /dst-path\"\n");
		string command = date->Io->in();
		if(command == "q")
		{
			*(bool*)argument = false;
			pthread_exit(NULL);
		}
		string src,dst;
		if (!TaskClass::rsolvepath(command,src,dst))
		{
			date->Io->out("path err, input again\n");
			continue;
		}
		*(bool*)argument = false;

		EventClass* repork = new EventClass;
		TaskClass * shelltask = new TaskClass(T_TFILE, command, &bdg_v,repork);
		
		shelltask->setwrbk(sendshell_cb);
		shelltask->setrdbk(tfile_cb);
		if(shelltask->rgstask() == -1)
		{
			delete repork;
			delete shelltask;
		}
		tasktimeout(0, 0, shelltask);
		repork->run();
	}

}

//--------------------------file----------------------------



//========================communicate==========================


void* communicate(void * argument)
{
	GlobalDate* date = GlobalDate::create();

	char st1[100] = {0};
	vector<string> v;

	date->Mysql->select_mysql("select id from devices where heart='y'",&v);
	sprintf(st1,"online devices about %d\n",v.size() );
	date->Io->out(st1);

	string she;
	while(1)
	{
		Pthread_x taskpthread;
		bool lock = true;
		date->Io->out("-q-  Quit\n");
		date->Io->out("-1-  translate files\n");
		date->Io->out("-2-  shell \n");
		date->Io->out("Your Choose :");
		she = date->Io->in();
		if(she == "1")
		{
			taskpthread.run(tfileworker, &lock);
			while(lock);
		}
		else if(she == "q")
		{
			exit(0);
		}
		else if(she == "2" )
		{
			taskpthread.run(shellworker, &lock);
			while(lock);
		}
		else
		{
			date->Io->out("choose illegal !\n");
		}
	}
}


//--------------------------communicate--------------------------
