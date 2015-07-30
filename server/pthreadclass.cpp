#include "pthreadclass.h"

Pthread_x::Pthread_x()
{
	
}

void Pthread_x::run(pth_fun fun,void* argument) 
{
	myfun = fun;
	int res = pthread_create(&mypthread,NULL,myfun,argument);
	if(res < 0 )
		perror("pthread create");
}

pthread_t Pthread_x::pthreadid() const
{
	return mypthread;
}

//------------------------------------------------------------------------------------------------------

Pthread_Pool::Pthread_Pool()
{
	
}


void Pthread_Pool::run(pth_fun fun,void* argument,int numb)
{
	myfun = fun;
	pthread_t pthid;
	int res;
	for (int i = 0; i < numb; ++i)
	{
		res = pthread_create(&pthid,NULL,myfun,argument);
		if(res < 0)
		{
			perror("pthread create");
			continue;
		}
		pthreadids.push_back(pthid);
	}
}
