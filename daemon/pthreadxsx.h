#ifndef PTHREADXSX_H_
#define PTHREADXSX_H_ 

#include <pthread.h>
#include <errno.h>
#include <stdio.h>


typedef void*(*pth_fun)(void*) ;

pthread_t pthread_run(pth_fun fun,void *argument);


#endif