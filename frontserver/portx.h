#ifndef PORTX_H_
#define PORTX_H_ 
#include "packet_x.h"


class st_heart
{
public:
	void init(int i,bool j);

	int fd;
	bool heart;
};

class Portx
{
public:
	Portx();
	~Portx();

	int addport(int fd);
	void eraseport(int fd,int epoolid);
	void shockheart(const PacketClass *buf);
	int getFd(int id);
	void outdieclient(int epoolid);
	void changeid(int oldid, int newid);

	
private:
	std::map<int , st_heart> id_fd;
	pthread_rwlock_t idLock;
};


#endif	