#include "socket_control.h"
#include "configx.h"
#include "pthread_class.h"
#include "Mysql_class.h"
#include "myio.h"
#include "pool.h"


void exit1();

int main(int argc, char const *argv[])
{

	atexit(exit1);
	Pthread_x timerkill;
	timerkill.run(timer_kill_zombie);

	Pthread_x communi;
	communi.run(communicate);

	mainlist();


	return 0;
}


void exit1()
{
	MysqlClass* data = MysqlClass::createsql();	
	delete data;
}