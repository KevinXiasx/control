#include "pthreadxsx.h"
#include <stdlib.h>

pthread_t pthread_run(pth_fun fun,void *argument)
{
	pthread_t  mypthread;
	int res = pthread_create(&mypthread,NULL,fun,argument);
	if(res < 0 )
		perror("pthread create");
	return mypthread;
}


/*int Timer(int sec, Callback back, void* argument)
{
	if( mybase == NULL)
		mybase = event_base_new();
	struct timeval t = {.tv_sec = sec, .tv_usec = 0 };
	struct event * eventpr = (struct event*)malloc(sizeof(struct event));
	evtimer_set(eventpr, back, eventpr);
	event_base_set(mybase, eventpr);
    evtimer_add(eventpr, &t);

    sleep(sec);
    back(sec,0,argument);
}*/