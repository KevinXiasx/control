#ifndef MESSAGE_H_
#define MESSAGE_H_


#include "projiec_head.h"

class Message
{
public:
	Message();
	Message(const U_MSG * m);    //be used in send to andriod
	~Message();

	int recvmsg(Address * add);	 //be uesd when recving msg form andriod

	int getId() const;			//retrun msg`id
	string getCom() const;		//if msg is translate file,this function return path of file
	int getSize() const;		//return length of command or size of file
private:
	U_MSG mymsg;
	string mycom;
};

#endif


#include <sys/stat.h>
#include "msgclass.h"

Message::Message(const U_MSG* m,string com)
{
	mymsg = *m;
	mycom = com;
}

int Message::recvmsg(Address * add)
{
	size_t si = recv(add->socketfd,&mymsg,sizeof(U_MSG),0);
	ERR(si,sizeof(U_MSG),"recv err",-1);
	return 0;
}


int Message::getSize() const
{
	switch( getId() )
	{
		case:ID_SHELL
			return mycom.size();
			break;
		case:ID_TFILE
			{
				struct stat buf;
				stat(mycom.c_str(),&buf);
				return (int)buf.st_size;
			}
			break;
		default:
			break;
	}
}

stirng Message::getCom() const
{
	return mycom;
}

int Message::getId() const
{
	return *(int*)(&mymsg);
}