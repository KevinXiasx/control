#ifndef XLOG_H_
#define XLOG_H_ 


#include "packet_x.h"


enum enumlog { ERR = 0, SEND = 1, RES = 2 };


class Xlog
{
public:

	void addLog(const PacketClass* form, enumlog l);
};

#endif


