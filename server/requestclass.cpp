
#include "requestclass.h"


RequestClass::RequestClass()
{
	pthread_mutex_init(&requestLock,NULL);
	pthread_cond_init(&requestCond,NULL);
}

RequestClass::~RequestClass()
{
	pthread_mutex_destroy(&requestLock);
	pthread_cond_destroy(&requestCond);
}


int RequestClass::insertrequest(const U_MSG *buf, int sock )
{
	int numb;
	pthread_mutex_lock(&requestLock);
	myrequest.insert(pair<int ,U_MSG>(sock,*buf));
	numb = myrequest.size();
	pthread_mutex_unlock(&requestLock);
	pthread_cond_signal(&requestCond);
	return numb;
}


int RequestClass::getrequest(U_MSG *res)
{
	int sock;
	std::map<int ,U_MSG>::iterator pr;
	pthread_mutex_lock(&requestLock);
	while(myrequest.size() == 0)
		pthread_cond_wait(&requestCond,&requestLock);
	pr = myrequest.begin();
	*res = pr->second;
	sock = pr->first;
	myrequest.erase(pr);
	pthread_mutex_unlock(&requestLock);
	return sock;
}