#ifndef IOCLASS_H_
#define IOCLASS_H_

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <stdarg.h>

using namespace std;

class IoClass
{
public:
	IoClass();
	~IoClass();
	
	string in();
	int out(string out);
	int err(string err);	

private:
	pthread_mutex_t outmutex;
	pthread_mutex_t errmutex;
	pthread_mutex_t inmutex;
	int outfd;
	int infd;
	int errfd;
};


#endif

