#include "taskclass.h"

TaskClass::TaskClass(U_MSG* taskhead, string command, vector<int> *id,EventClass* manager)
{
	mymsg = *taskhead;
	mymanager = manager;
	mycommand = command;
	plannedid = *id;
}