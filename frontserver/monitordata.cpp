#include "monitordata.h"


datamutex::datamutex()
{
	date = 0;
	pthread_mutex_init(&mutex,NULL);
}
datamutex::~datamutex()
{
	pthread_mutex_destroy(&mutex);
}

int datamutex::dosome(produce i)
{
	int k;
	pthread_mutex_lock(&mutex);
	if( i==ADD )
	{
		k = ++date;
	}
	else if(i==DEL)
	{
		k = --date;
	}
	else
	{
		k = date;
	}
	pthread_mutex_unlock(&mutex);
	return k;	
}

