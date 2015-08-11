#ifndef WORKER_H_
#define WORKER_H_ 
#include "../include/socketcontrol.h"
#include "../include/globaldate.h"
#include "../include/encrypt.h"
#include "../include/bdgmgerclass.h"

#define KILLTIME 3600

int networker();
void* communicate(void * argument);
void* shellworker(void * argument);


void accpet_cb(int sock, short event, void* arg);
void killer(int sock, short event, void* arg);
void read_cb(int sock, short event, void* arg);

#endif
