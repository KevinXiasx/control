#ifndef POOL_H_
#define POOL_H_ 

#include "project_head.h"
#include "msgclass.h"
#include "pthread_class.h"
#include "socket_control.h"

int mainlist();
void* guard(void* argument);

void* forward(void* argument);


void* communicate(void * argument);


#endif