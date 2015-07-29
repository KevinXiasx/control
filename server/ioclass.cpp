#include "ioclass.h"

IoClass::IoClass()
{
	pthread_mutex_init(&outmutex,NULL);
	pthread_mutex_init(&errmutex,NULL);
	pthread_mutex_init(&inmutex,NULL);

	infd = STDIN_FILENO;
	outfd = STDOUT_FILENO;
	errfd = STDERR_FILENO;
}


IoClass::~IoClass()
{
	pthread_mutex_destroy(&outmutex);
	pthread_mutex_destroy(&errmutex);
	pthread_mutex_destroy(&inmutex);
}


string IoClass::in()
{
	//
	out("\nkevin@: ");
	//
	char tmp[1024] = {0};
	pthread_mutex_lock(&inmutex);
	int seek = read(infd,tmp,1024);
	pthread_mutex_unlock(&inmutex);
	*(tmp+seek-1) = 0;
	string k = tmp;
	return k;
}

int IoClass::out(string out)
{

	pthread_mutex_lock(&outmutex);
	int n = write(outfd,out.c_str(),out.size());
	pthread_mutex_unlock(&outmutex);
	return n;
}

int IoClass::err(string err)
{
	pthread_mutex_lock(&errmutex);
	int n = write(errfd,err.c_str(),err.size());
	pthread_mutex_unlock(&errmutex);
	return n;
}