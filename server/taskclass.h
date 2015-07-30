#ifndef TASKCLASS_H_
#define TASKCLASS_H_ 

#include <vector>
#include "project_head.h"
#include <iostream>
#include "eventclass.h"
using namespace std;

class TaskClass
{
public:
	TaskClass(U_MSG* taskhead, string command, vector<int> *id,EventClass* manager);
	
	~TaskClass();

	U_MSG mymsg;
	string mycommand;
	EventClass* mymanager;
	vector<int> plannedid;
	vector<int> failedid;
	vector<int> successfulid;
};


#endif

