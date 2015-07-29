#include <event.h>
#include <stdio.h>
#include <stdlib.h>




int main(int argc, char const *argv[])
{
	struct event_base* mybase;
	mybase = event_base_new();

	struct event * eventpr = (struct event*)malloc(sizeof(struct event));
	event_set(eventpr, fd, EV_READ|EV_PERSIST, back, NULL);

	event_base_set(mybase, eventpr);
	event_add(eventpr, NULL);

	event_base_dispatch(mybase);

	return 0;
}