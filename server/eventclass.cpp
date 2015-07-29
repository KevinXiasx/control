#include "eventclass.h"
#include <pthread.h>
#include "project_head.h"
EventClass::EventClass()
{
	mybase = event_base_new();
	pthread_mutex_init(&mymutex,NULL);
}

EventClass::~EventClass()
{
	pthread_mutex_destroy(&mymutex);
	event_base_free(mybase);
}


int EventClass::createread(int fd, Callback back, void* arg)
{
	struct event * eventpr = (struct event*)malloc(sizeof(struct event));
	event_set(eventpr, fd, EV_READ|EV_PERSIST, back, arg);
	pthread_mutex_lock(&mymutex);
	event_base_set(mybase, eventpr);
	event_add(eventpr, NULL);
	myevent.insert(pair<int , struct event *>(fd,eventpr));
	pthread_mutex_unlock(&mymutex);

}

int EventClass::createwrite(int fd, Callback back, void* arg)
{
	struct event * eventpr = (struct event*)malloc(sizeof(struct event));
	event_set(eventpr, fd, EV_WRITE, back, arg);

	pthread_mutex_lock(&mymutex);
	event_base_set(mybase, eventpr);
	event_add(eventpr, NULL);

	myevent.insert(pair<int , struct event *>(fd,eventpr));
	pthread_mutex_unlock(&mymutex);
}

int EventClass::run()
{
	event_base_dispatch(mybase);
}

int EventClass::erasevent(int fd)
{
	pthread_mutex_lock(&mymutex);
	map<int , struct event *>::iterator pr = myevent.find(fd);
	pthread_mutex_unlock(&mymutex);
	
	event_del(pr->second);
	free(pr->second);


}

int EventClass::createtimer(int sec,Callback back)
{
	struct timeval t = {.tv_sec = sec, .tv_usec = 0 };
	struct event * eventpr = (struct event*)malloc(sizeof(struct event));
	evtimer_set(eventpr, back, 0);
	event_base_set(mybase, eventpr);
    evtimer_add(eventpr, &t);
}