#ifndef MONITORDATE_H_
#define MONITORDATE_H_ 
#include "packet_x.h"

enum produce { ADD = 1, DEL = 2, RETURN = 3 };


class datamutex
{
public:
	datamutex();
	~datamutex();

	int dosome(produce i);

private:
	int date;
	pthread_mutex_t mutex;
};



class Monitordata
{
public:

	datamutex onlineNumb;
	datamutex onlineUser;

	datamutex resPacket;
	datamutex errPacket;

	datamutex sendPacket;

	datamutex pthreadnumbDepacket;

};



#endif


