#ifndef REQUESTCLASS_H_
#define REQUESTCLASS_H_

#include "project_head.h"
#include <map>


class RequestClass
{
public:
	RequestClass();
	~RequestClass();

	int insertrequest(const U_MSG *buf, int sock);
	int getrequest(U_MSG *res);
	//int getrequest(U_MSG *res,int sec);

private:
	std::map<int ,U_MSG> myrequest;
	pthread_mutex_t requestLock;
	pthread_cond_t requestCond;
};


#endif

