#ifndef WORKER_H_
#define WORKER_H_ 
#include "socketcontrol.h"
#include "globaldate.h"
#include "encrypt.h"

void* requestworker(void * argument);

int networker();

void* communicate(void * argument);

#define KILLTIME 3600
void killer(int sock, short event, void* arg);

#endif
