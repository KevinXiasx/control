#include "../include/eventclass.h"
#include <pthread.h>
#include "../include/project_head.h"


EventClass::EventClass()
{
	mybase = event_base_new();
	rdsize = 0;
	wrsize = 0;
}

EventClass::~EventClass()
{
	event_base_free(mybase);
}


struct event * EventClass::createread(int fd, Callback back, void* arg)
{
	struct event * eventpr = (struct event*)malloc(sizeof(struct event));
	event_set(eventpr, fd, EV_READ|EV_PERSIST, back, arg);
	event_base_set(mybase, eventpr);
	event_add(eventpr, NULL);
	rdsize++;
	return eventpr;
}

struct event * EventClass::createwrite(int fd, Callback back, void* arg)
{
	struct event * eventpr = (struct event*)malloc(sizeof(struct event));
	event_set(eventpr, fd, EV_WRITE, back, arg);

	event_base_set(mybase, eventpr);
	event_add(eventpr, NULL);
	wrsize++;
	return eventpr;
}

int EventClass::run()
{
	event_base_dispatch(mybase);
}


int EventClass::createtimer(int sec, Callback back, void* arg)
{
	struct timeval t = {.tv_sec = sec, .tv_usec = 0 };
	struct event * eventpr = (struct event*)malloc(sizeof(struct event));
	if( arg == NULL)
		evtimer_set(eventpr, back, eventpr);
	else
		evtimer_set(eventpr, back, arg);
	event_base_set(mybase, eventpr);
    evtimer_add(eventpr, &t);
}


int EventClass::getrdsize() const
{
	return rdsize;
}
int EventClass::getwrsize() const
{
	return wrsize;
}


int EventClass::deletevt(int b )
{
	if( b == WRITEVT )
		wrsize--;
	if( b == READEVT)
		rdsize--;
}