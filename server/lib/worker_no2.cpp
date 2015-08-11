#include "../include/worker.h"
#include "../include/project_head.h"
#include "../include/pthreadclass.h"
 #include <sys/sendfile.h>

//========================shell==========================

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
			Bridge* t = date->Bdgmger->getbdg(atoi(in1.c_str()),KEY_ID);
			if(t==NULL)
			{
				date->Io->out("this id hasn`t device !\n");
				return false;
			}
			date->Io->out("1 devise is avtive and  been choosed!\n");
			bdg_v->push_back(t);
			return true;
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
	DEBUGW;
	int n = sendpt(sock,pak->prt,sizeof(U_MSG));
	n = sendpt(sock,pak->prt+sizeof(U_MSG),pak->size-sizeof(U_MSG));
	DEBUGW;
	if(n != pak->size-sizeof(U_MSG))
	{
		DEBUGW;
		date->Bdgmger->erasebdg(sock,KEY_SOCK,FLAG_NODEL);
		pak->bdg->close();
		if(n < 0 )
		{
			DEBUGW;
			pak->task->failbdg(pak->bdg);
			perror("send head:");
			date->Io->err("data send fail\n");
			goto reportlabel;
		}
		else
		{
			DEBUGW;
			pak->task->failbdg(pak->bdg);
			date->Io->err("date send imperfect\n");			
			goto reportlabel;
		}
	}
	pak->bdg->outevt(FLAG_WRITE);
	delete pak;
	DEBUGW;
	return ;
reportlabel:
	DEBUGW;
	if( pak->task->over() )
	{
		DEBUGW;
		char result[100] = {0};
		sprintf(result,"Task has done,%d devices success, %d devices fial and them id save in database",pak->task->succsnum(),pak->task->failnum());
		date->Io->out(result);
		pak->task->ending();
		delete pak->task;
		delete pak;
		pthread_exit(NULL);
	}
	else
	{
		DEBUGW;
		delete pak;
	}
}

void reportshell_cb(int sock, short event, void* arg)
{
	GlobalDate* date = GlobalDate::create();
	TaskClass* task = (TaskClass*)arg;
	Bridge* bdg = date->Bdgmger->getbdg(sock,KEY_SOCK);
	U_MSG rep;
	DEBUGW;
	int n = recvpt(sock,&rep,sizeof(U_MSG));
	DEBUGW;
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
			char filename[20] = {0};
			sprintf(filename, "log/log%d", bdg->id());

			int logfilefd = open(filename, O_CREAT|O_APPEND|O_RDWR, 0644);
/*			if(logfilefd != -1)
				decpipe(sock, logfilefd, rep.answer_m.loglen);*/

			struct timeval timeout;
			timeout.tv_sec = 2;
			timeout.tv_usec= 0;
			socklen_t len = sizeof(timeout);
			int result = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout.tv_sec, len);

			char buf[4096];
			int j, k, i=0;
			DEBUGM("shell t begin");
			while( (j=read(sock, buf, 4096)) > 0)
			{
				if( (k=write(logfilefd, buf, j)) <= 0)
					break;
				if( (i=i+j) == rep.answer_m.loglen)
					break;
			}
			DEBUGM("shell t over");
			if(k<0 || j<=0)
			{
				DEBUGW;
				date->Bdgmger->erasebdg(sock,KEY_SOCK,FLAG_NODEL);
				task->failbdg(bdg);
				bdg->close();
			}
			else{
				DEBUGW;
				task->succsbdg(bdg);
			}
			close(logfilefd);
		}
		else{
			DEBUGW;
			task->failbdg(bdg);
		}
	}
	DEBUGW;
	DEBUGI(bdg->intoevt(date->Event, read_cb, FLAG_READ,bdg));
	DEBUGW;
	if(task->over() )
	{
		DEBUGW;
		char result[100] = {0};
		sprintf(result,"Task has done,%d devices success, %d devices fial and them id save in database",task->succsnum(),task->failnum());
		date->Io->out(result);
		task->ending();
		delete task;
		pthread_exit(NULL);
	}
}


void* shellworker(void * argument)
{
	GlobalDate* date = GlobalDate::create();

	vector<Bridge*> bdg_v;
	if(!getdevice(&bdg_v))
		return NULL;

	date->Io->out("please input shell command !\n");
	string command = date->Io->in();

	EventClass* repork = new EventClass;
	TaskClass * shelltask = new TaskClass(T_SHELL, command, &bdg_v,repork);
	shelltask->setwrbk(sendshell_cb);
	shelltask->setrdbk(reportshell_cb);
	shelltask->rgstask();
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

			DEBUGM("tfile begin");
			while( (n=read(fd, filebuf, 4096)) > 0)
				if( (m=sendpt(sock, filebuf, n)) <= 0 )
				{
					DEBUGI(m);
					break;
				}
			DEBUGM("tfile over");
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
	DEBUGI(bdg->intoevt(date->Event, read_cb, FLAG_READ,bdg));
tfilereportlabel:
	if( task->over() )
	{
		char result[100] = {0};
		sprintf(result,"Task has done,%d devices success, %d devices fial and them id save in database",task->succsnum(),task->failnum());
		date->Io->out(result);
		task->ending();
		delete task;
		pthread_exit(NULL);
	}
}


void* tfileworker(void* argument)
{
	GlobalDate* date = GlobalDate::create();

	vector<Bridge*> bdg_v;
	if(!getdevice(&bdg_v))
		return NULL;
	while(1)
	{
		date->Io->out("please input \"/sourse-path to /dst-path\"\n");
		string command = date->Io->in();
		if(command == "q")
			pthread_exit(NULL);
		string src,dst;
		if (!TaskClass::rsolvepath(command,src,dst))
		{
			date->Io->out("path err, input again\n");
			continue;
		}

		EventClass* repork = new EventClass;
		TaskClass * shelltask = new TaskClass(T_TFILE, command, &bdg_v,repork);
		shelltask->setwrbk(sendshell_cb);
		shelltask->setrdbk(tfile_cb);
		if(shelltask->rgstask() == -1)
		{
			delete repork;
			delete shelltask;
		}

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
		date->Io->out("-q-  Quit\n");
		date->Io->out("-1-  translate files\n");
		date->Io->out("-2-  shell \n");
		date->Io->out("Your Choose :");
		she = date->Io->in();
		if(she == "1")
		{
			taskpthread.run(tfileworker);
			pthread_t pthid = taskpthread.pthreadid();
			//date->Event->createtimer(30,killself);
			pthread_join(pthid,NULL);
		}
		else if(she == "q")
		{
			exit(0);
		}
		else if(she == "2" )
		{
			taskpthread.run(shellworker);
			pthread_t pthid = taskpthread.pthreadid();
			//date->Event->createtimer(30,killself);
			pthread_join(pthid,NULL);
		}
		else
		{
			date->Io->out("choose illegal !\n");
		}
	}
}


//--------------------------communicate--------------------------
