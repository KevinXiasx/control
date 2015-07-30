#include "socketcontrol.h"
#include "configx.h"
#include "pthreadclass.h"
#include "mysqlclass.h"
#include "ioclass.h"
#include "globaldate.h"
#include "worker.h"


void exitcall();

int main(int argc, char const *argv[])
{
	GlobalDate* date = GlobalDate::create();
	if(!date->check())
		return 0;
	killer(0,0,NULL);
	//date->Event.createtimer(KILLTIME,killer);
	atexit(exitcall);
	
	Pthread_x communi;
	communi.run(communicate);
	Pthread_x reqworker;
	reqworker.run(requestworker);
	
	networker();
	

	//communicate(NULL);
	return 0;
}


void exitcall()
{
	GlobalDate* date = GlobalDate::create();	
	delete date;
}