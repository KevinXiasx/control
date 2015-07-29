#ifndef PTHREAD_CLASS_H_
#define PTHREAD_CLASS_H_ 

#include <vector>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>

using namespace std;

typedef void*(*pth_fun)(void*) ;

class Pthread_x
{
public:
	Pthread_x();

	void run(pth_fun fun,void* argument = NULL) ;
	pthread_t pthreadid() const;
private:
	pthread_t mypthread;
	pth_fun myfun;
};


class Pthread_Pool
{
public:
	Pthread_Pool();

	void run(pth_fun fun,void* argument = NULL,int numb = 1);
private:
	pth_fun myfun;
	std::vector<pthread_t> pthreadids;
};

#endif
