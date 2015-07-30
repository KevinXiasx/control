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

	int createread(int fd, Callback back, void* arg = NULL);
	int createwrite(int fd, Callback back, void* arg = NULL);

	int eraseread(int fd);
	int erasewrite(int fd);

	int run();

private:
	map<int , struct event *> myreadevent;
	map<int , struct event *> mywritevent;
	struct event_base* mybase;
	pthread_mutex_t mymutex;
};

#endif


