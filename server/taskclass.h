#ifndef TASKCLASS_H_
#define TASKCLASS_H_ 

#include <vector>
#include "project_head.h"
#include <iostream>
#include "eventclass.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

class TaskClass;
class Bridge;


class Pak
{
public:
	Pak(void* prt,size_t size,TaskClass* task,Bridge* bdg);
	~Pak();

	void* prt;
	size_t size;
	TaskClass* task;
	Bridge* bdg;
};




class TaskClass
{
public:
	TaskClass(int type, string command, vector<Bridge*> *bdg, EventClass* manager);
	~TaskClass();

	int setrdbk(Callback rd);
	int setwrbk(Callback wr);

	int succsbdg(Bridge* bdg);
	int failbdg(Bridge* bdg);

	int rgstask();

	bool over();
	int failnum();
	int succsnum();
	int ending();

	static bool rsolvepath(const string& command, string& source, string& dst);

	int srcstring(string & src);

private:
	int mytype;
	string mycommand;
	EventClass* mymanager;

	vector<Bridge*> *mybdg;
	vector<Bridge*> failedbdg;
	vector<Bridge*> succsedbdg;

	Callback myrdback;
	Callback mywrback;
	Pak* pak;
};


#endif
