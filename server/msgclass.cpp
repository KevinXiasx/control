#include <sys/stat.h>
#include "msgclass.h"

Message::Message()
{

}

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
		case ID_SHELL:
			return mycom.size();
			break;
		case ID_TFILE:
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

string Message::getCom() const
{
	return mycom;
}

int Message::getId() const
{
	return *(int*)(&mymsg);
}

Message::~Message()
{

}