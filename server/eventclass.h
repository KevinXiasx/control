#ifndef EVENTCLASS_H_
#define EVENTCLASS_H_ 

#include <map>
#include <event.h>


using namespace std;

typedef  void (*Callback)(int sock, short event, void* arg);


class EventClass
{
public:
	EventClass();
	~EventClass();
	int createtimer(int sec,Callback back);

	struct event * createread(int fd, Callback back, void* arg = NULL);
	struct event * createwrite(int fd, Callback back, void* arg = NULL);

	int run();

private:
	struct event_base* mybase;
};

#endif


