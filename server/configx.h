#ifndef CONFIGX_H_
#define CONFIGX_H_ 

#include <cstring>
#include <fstream>
#include <map>
#include <vector>
#include <list>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <algorithm>

using namespace std;

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