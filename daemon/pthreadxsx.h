#ifndef PTHREADXSX_H_
#define PTHREADXSX_H_ 

#include <pthread.h>
#include <errno.h>
#include <stdio.h>

extern struct event_base* mybase;

typedef void*(*pth_fun)(void*) ;
typedef void (*Callback)(int sock, short sh, void* argument);

pthread_t pthread_run(pth_fun fun,void *argument);


/*int Timer(int sec, Callback back, void* argument);*/


#endif

