#include "sockmgerclass.h"

int SockmgerClass::getsock(int id)
{
	std::map<int, int>::iterator i; 
	i = id_sock.find(id);
	if ( i == id_sock.end() )
		return -1;
	return i->second;
}

bool SockmgerClass::addsock(int id, int sock)
{
	std::map<int, int>::iterator pr;
	pr = id_sock.find(id);
	if( pr != id_sock.end() )
		return false;
	sock_id.insert(pair<int,int>(sock,id));
	id_sock.insert(pair<int ,int >(id,sock));
	return true;
}

bool SockmgerClass::erasesock(int sock)
{
	for (map<int, int>::iterator i = id_sock.begin(); i !=id_sock .end(); ++i)
	{
		if(i->second == sock)
		{	
			id_sock.erase(i);
			sock_id.erase(sock);
			DEBUGM("erase sock from sockmger ");
			return true;
		}
	}
	return false;
}

SockmgerClass::SockmgerClass()
{
	pthread_mutex_init(&mymutex,NULL);
}
SockmgerClass::~SockmgerClass()
{
	pthread_mutex_destroy(&mymutex);
}