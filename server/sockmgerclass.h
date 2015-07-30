#ifndef SOCKMGERCLASS_H_
#define SOCKMGERCLASS_H_ 

#include "project_head.h"
#include <map>


class SockmgerClass
{
public:
	SockmgerClass();
	~SockmgerClass();

	int getsock(int id);

	bool addsock(int id, int sock);
	bool erasesock(int sock);

private:
	std::map<int, int> id_sock;
	std::map<int, int> sock_id;
	pthread_mutex_t mymutex;
};


#endif


