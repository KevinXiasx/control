#ifndef EVENTCLASS_H_
#define EVENTCLASS_H_ 

#include <map>
#include <event.h>
#include "../include/project_head.h"

using namespace std;

#define WRITEVT 1
#define READEVT 2

class EventClass
{
public:
	EventClass();
	~EventClass();
	int createtimer(int sec,Callback back, void* arg = NULL);

	struct event * createread(int fd, Callback back, void* arg = NULL);
	struct event * createwrite(int fd, Callback back, void* arg = NULL);

	int deletevt(int );

	int getrdsize() const;
	int getwrsize() const;
	int run();

private:
	struct event_base*  mybase;
	int rdsize;
	int wrsize;
};

#endif


