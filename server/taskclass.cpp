#include "taskclass.h"
#include "bdgmgerclass.h"

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
}

TaskClass::~TaskClass()
{

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
			void * p = malloc(sizeof(U_MSG)+mycommand.size());
			U_MSG* msg = (U_MSG*)p;
			msg->type = mytype;
			switch(mytype)
			{
				case T_SHELL:
					msg->shell_m.commandlen = mycommand.size();
					msg->shell_m.id = (*pr)->id();
					memcpy(msg+1,mycommand.c_str(),mycommand.size());
					break;
				default:
					break;
			}
			pak = new Pak(p,sizeof(U_MSG)+mycommand.size(),this,*pr);
			(*pr)->intoevt(mymanager,mywrback,FLAG_WRITE,pak);
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

int TaskClass::ending()
{
	vector<Bridge*>::iterator pr;
	for(pr = mybdg->begin(); pr!=mybdg->end(); pr++)
	{
		if((*pr)->socket() == Unknow)
			delete *pr;
	}
}
