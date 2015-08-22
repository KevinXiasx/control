#include "../include/taskclass.h"
#include "../include/bdgmgerclass.h"
#include "../include/worker.h"

Pak::Pak(void* prt,size_t size, TaskClass* task,Bridge* bdg)
{
	this->prt = prt;
	this->size = size;
	this->task = task;
	this->bdg = bdg;
}
Pak::~Pak()
{
	free(prt);
}



TaskClass::TaskClass(int type, string command, vector<Bridge*> *bdg, EventClass* manager)
{
	mytype = type;
	mycommand = command;
	mybdg = bdg;
	mymanager = manager;
	pak = NULL;
	myrdback = NULL;
	mywrback = NULL;
	write_totle = 0;
}

TaskClass::~TaskClass()
{
	delete mymanager;
}

int TaskClass::setrdbk(Callback rd)
{
	myrdback = rd;
}

int TaskClass::setwrbk(Callback wr)
{
	mywrback = wr;
}


int TaskClass::rgstask()
{
	vector<Bridge*>::iterator pr;
	for(pr = mybdg->begin(); pr!=mybdg->end(); pr++)
	{
		if(mywrback!=NULL)
		{
			void * p = NULL;
			switch(mytype)
			{
				case T_SHELL:
				{
					p = malloc(sizeof(U_MSG)+mycommand.size());
					U_MSG* msg = (U_MSG*)p;
					msg->type = mytype;
					msg->shell_m.commandlen = mycommand.size();
					msg->shell_m.id = (*pr)->id();
					memcpy(msg+1,mycommand.c_str(),mycommand.size());
					pak = new Pak(p,sizeof(U_MSG)+mycommand.size(),this,*pr);
					(*pr)->intoevt(mymanager,mywrback,FLAG_WRITE,pak);
					break;
				}
				case T_TFILE:
				{
					string src,dst;
					rsolvepath(mycommand,src,dst);
					p = malloc(sizeof(U_MSG)+dst.size());
					U_MSG* msg = (U_MSG*)p;
					msg->type = mytype;
					msg->tfile_m.id = (*pr)->id();
					msg->tfile_m.mode = 600;
					msg->tfile_m.path = dst.size();
					struct stat statbuf;
					int res = stat(src.c_str(),&statbuf);
					if(res == -1)
						return -1;
					msg->tfile_m.size = statbuf.st_size;
					memcpy(msg+1,dst.c_str(),dst.size());
					pak = new Pak(p,sizeof(U_MSG)+dst.size(),this,*pr);
					(*pr)->intoevt(mymanager,mywrback,FLAG_WRITE,pak);
					break;
				}
				default:
					break;
			}

		}
		if(myrdback!=NULL)
		{
			(*pr)->intoevt(mymanager,myrdback,FLAG_READ,this);
		}
	}
} 



int TaskClass::succsbdg(Bridge* bdg)
{
	succsedbdg.push_back(bdg);
}
int TaskClass::failbdg(Bridge* bdg)
{
	failedbdg.push_back(bdg);
}


bool TaskClass::over()
{
	if(failedbdg.size() + succsedbdg.size() == mybdg->size())
		return true;
	return false;
}

int TaskClass::failnum()
{
	return failedbdg.size();
}
int TaskClass::succsnum()
{
	return succsedbdg.size();
}

int TaskClass::ending(EventClass * evt)
{
	vector<Bridge*>::iterator pr;
	for(pr = mybdg->begin(); pr!=mybdg->end(); pr++)
	{
		if((*pr)->socket() == Unknow)
			delete *pr;
		else
			(*pr)->intoevt(evt, read_cb, FLAG_READ, (*pr));
	}
}

int TaskClass::srcstring(string & src)
{
	string::size_type loc = mycommand.find("/",0);
	string::size_type loc2 = mycommand.find(" ",loc);
	string::size_type loc3 = mycommand.find("/",loc2);
	if(loc == string::npos || loc2 == string::npos || loc3 == string::npos)
		return false;
	src = mycommand.substr(loc,loc2-loc);
/*	dst = mycommand.substr(loc3);
	int tmpfd = open(sourse.c_str(), O_RDONLY);
	if( tmpfd < 0)
		return false;
	close(tmpfd);
	return true;	*/
}


bool TaskClass::rsolvepath(const string& command, string& source, string& dst)
{
	string::size_type loc = command.find("/",0);
	string::size_type loc2 = command.find(" ",loc);
	string::size_type loc3 = command.find("/",loc2);
	if(loc == string::npos || loc2 == string::npos || loc3 == string::npos)
		return false;
	source = command.substr(loc,loc2-loc);
	dst = command.substr(loc3);
	int tmpfd = open(source.c_str(), O_RDONLY);
	if( tmpfd < 0)
		return false;
	close(tmpfd);
	return true;
}

