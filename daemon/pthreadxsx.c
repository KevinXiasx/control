#include "pthreadxsx.h"


pthread_t pthread_run(pth_fun fun,void *argument)
{
	pthread_t  mypthread;
	int res = pthread_create(&mypthread,NULL,fun,argument);
	if(res < 0 )
		perror("pthread create");
	return mypthread;
}

