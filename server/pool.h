#ifndef POOL_H_
#define POOL_H_ 

#include "project_head.h"
#include "pthread_class.h"
#include "socket_control.h"

int mainlist();


void* communicate(void * argument);

void* timer_kill_zombie(void* argument);
#endif