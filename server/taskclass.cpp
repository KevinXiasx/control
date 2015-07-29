#include "taskclass.h"

TaskClass::TaskClass(U_MSG* taskhead, string command, vector<int> *id)
{
	U_MSG mymsg = *taskhead;

	string mycommand = command;
	vector<int> plannedid = *id;
}