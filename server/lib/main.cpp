#include "../include/socketcontrol.h"
#include "../include/configx.h"
#include "../include/pthreadclass.h"
#include "../include/mysqlclass.h"
#include "../include/ioclass.h"
#include "../include/globaldate.h"
#include "../include/worker.h"


void exitcall();

int main(int argc, char const *argv[])
{
	GlobalDate* date = GlobalDate::create();
	if(!date->check())
		return 0;
//	killer(0,0,NULL);
	atexit(exitcall);
	
	Pthread_x communi;
	communi.run(communicate);
	
	networker();
	return 0;
}


void exitcall()
{
	GlobalDate* date = GlobalDate::create();	
	delete date;
}