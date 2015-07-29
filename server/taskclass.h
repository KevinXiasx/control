#ifndef TASKCLASS_H_
#define TASKCLASS_H_ 

#include <vector>
#include "project_head.h"
#include <iostream>

using namespace std;

class TaskClass
{
public:
	TaskClass(U_MSG* taskhead, string command, vector<int> *id);
	
	~TaskClass();

	U_MSG mymsg;
	string mycommand;
	vector<int> plannedid;
	vector<int> failedid;
	vector<int> successfulid;
};


#endif

