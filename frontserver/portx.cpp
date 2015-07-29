#include "portx.h"
#include "globalDate.h"

Portx::Portx()
{
	pthread_rwlock_init(&idLock,NULL);
}
Portx::~Portx()
{
	pthread_rwlock_destroy(&idLock);
}

int Portx::addport(int fd)
{
	static int i=-1;
	pthread_rwlock_wrlock(&idLock);
	st_heart heart;
	heart.init(fd,true);

	while( id_fd.find(i) != id_fd.end() )//临时连接用户，给予临时ID
	{
		i--;
		if(i > 0)
			i = -1;
	}

	id_fd.insert(pair<int,st_heart>(i,heart));
	pthread_rwlock_unlock(&idLock);
	return i;
}

void Portx::shockheart(const PacketClass *buf)
{
	pthread_rwlock_wrlock(&idLock);
	st_heart heart;
	heart.init(buf->getFd(),true);
	id_fd[buf->getId()] = heart;
	pthread_rwlock_unlock(&idLock);
}

void st_heart::init(int i,bool j)
{
	fd = i;
	heart = j;
}

int Portx::getFd(int id)
{
	std::map<int , st_heart>:: iterator pr;
	pr = id_fd.find(id);
	if( pr == id_fd.end() )
		return -1;
	return pr->second.fd;
}


void Portx::outdieclient(int epoolid)
{
	GlobalDate* globaldate= GlobalDate::create();
	std::map<int , st_heart>::iterator pr;
	for(pr = id_fd.begin(); pr!= id_fd.end(); pr++)
	{
		if(!(pr->second.heart))
		{
			close(pr->second.fd);
			struct epoll_event event;
			event.events = EPOLLIN|EPOLLET;
			event.data.fd = pr->second.fd;
			epoll_ctl(epoolid,EPOLL_CTL_DEL,pr->second.fd,&event);
			if(pr->first > 0 )
				globaldate->monitordata.onlineUser.dosome(DEL);
			id_fd.erase(pr);
			globaldate->monitordata.onlineNumb.dosome(DEL);
			cout<<"T die one !"<<endl;
		}
		else
			pr->second.heart = false;
	}
}

void Portx::eraseport(int fd,int epoolid)
{
	GlobalDate* globaldate= GlobalDate::create();
	close(fd);
	struct epoll_event event;
	event.events = EPOLLIN|EPOLLET;
	event.data.fd =fd;
	epoll_ctl(epoolid,EPOLL_CTL_DEL,fd,&event);
}
void Portx::changeid(int oldid, int newid)
{
	st_heart heart;
	heart.init( (id_fd.find(oldid)->second).fd,true);
	id_fd.erase(id_fd.find(oldid));
	id_fd.insert(pair<int,st_heart>(newid,heart));
}