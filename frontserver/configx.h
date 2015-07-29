#ifndef CONFIGX_H_
#define CONFIGX_H_ 
#include "packet_x.h"
class Configx
{
public:
	Configx();
	~Configx();


	int getconfint(string name);
	string getconfig(string name);

	bool admen(string name,string value);

	std::vector<string> stdconfig;
private:
	std::map<string, string> myConfig;
	pthread_rwlock_t configLock;

	


};


#endif