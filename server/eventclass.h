#ifndef EVENTCLASS_H_
#define EVENTCLASS_H_ 

#include <map>
#include <event.h>
#include "project_head.h"

using namespace std;


class EventClass
{
public:
	EventClass();
	~EventClass();
	int createtimer(int sec,Callback back);

	struct event * createread(int fd, Callback back, void* arg = NULL);
	struct event * createwrite(int fd, Callback back, void* arg = NULL);

	int getrdsize() const;
	int getwrsize() const;
	int run();

private:
	struct event_base* mybase;
	int rdsize;
	int wrsize;
};

#endif


