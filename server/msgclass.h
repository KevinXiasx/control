#ifndef MESSAGE_H_
#define MESSAGE_H_


#include "project_head.h"
#include "socket_control.h"

class Message
{
public:
	Message();
	~Message();
	Message(const U_MSG* m,string com);     //be used in send to andriod

	int recvmsg(Address * add);				//be uesd when recving msg form andriod

	int getId() const;						//retrun msg`id
	string getCom() const;					//if msg is translate file,this function return path of file
	int getSize() const;					//return length of command or size of file
private:
	U_MSG mymsg;
	string mycom;
};

#endif


