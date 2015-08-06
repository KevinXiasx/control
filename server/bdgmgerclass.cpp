#include "bdgmgerclass.h"
#include "eventclass.h"
#include <unistd.h>
Bridge::Bridge()
{
	mysock = Unknow;
	myid = Unknow;
	myrdevt = NULL;
	mywtevt = NULL;
	mybase = NULL;
}

Bridge::Bridge(SOCK sock, ID id)
{
	mysock = sock;
	myid = id;
	myrdevt = NULL;
	mywtevt = NULL;
	mybase = NULL;		
}

Bridge::Bridge(SOCK sock)
{
	mysock = sock;
	myid = Unknow;
	myrdevt = NULL;
	mywtevt = NULL;
	mybase = NULL;		
}

Bridge::Bridge(SOCK sock, EventClass * eventmg, Callback backfun, int flag, void* argument)
{
	assert(flag != FLAG_BOTH);
	mysock = sock;
	myid = Unknow;
	myrdevt = NULL;
	mywtevt = NULL;
	mybase = eventmg;
	if(flag == FLAG_READ)
		myrdevt = eventmg->createread(mysock, backfun, argument);
	else if(flag == FLAG_WRITE)
		mywtevt = eventmg->createwrite(mysock, backfun, argument);
}

Bridge::~Bridge()
{
	printf("out one bdg\n");
	this->close();
}

SOCK Bridge::socket() const
{
	return mysock;
}

ID Bridge::id() const
{
	return myid;
}


ID Bridge::setid(ID id)
{
	myid = id;
	return myid;
}

int Bridge::intoevt(EventClass * eventmg, Callback backfun, int flag, void* argument)
{
	if(mysock == Unknow)
		return NOEXIST;
	assert(flag != FLAG_BOTH);
	if(flag == FLAG_READ)
	{
		if(myrdevt != NULL)
			outevt(FLAG_READ);
		myrdevt = eventmg->createread(mysock, backfun, argument);
	}
	else if(flag == FLAG_WRITE)
	{
		if(mywtevt != NULL)
			outevt(FLAG_WRITE);		
		mywtevt = eventmg->createwrite(mysock, backfun, argument);
	}
	mybase = eventmg;
}

int Bridge::outevt(int flag)
{
	mybase = NULL;
	if(flag == FLAG_READ && myrdevt != NULL)
	{
		assert(myrdevt != NULL);
		event_del(myrdevt);
		free(myrdevt);
		myrdevt = NULL;
	}
	else if(flag == FLAG_WRITE && mywtevt != NULL)
	{
		event_del(mywtevt);
		free(mywtevt);
		mywtevt = NULL;
	}
	else if( flag == FLAG_BOTH )
	{
		if(myrdevt != NULL)
		{
			event_del(myrdevt);
			free(myrdevt);
			myrdevt = NULL;	
		}
		if(mywtevt != NULL)
		{
			event_del(mywtevt);
			free(mywtevt);
			mywtevt = NULL;			
		}		
	}
}


int Bridge::close()
{
	outevt(FLAG_BOTH);
	::close(mysock);
	mysock = Unknow;
}


//=====================================================================



Bridge* BdgmgerClass::getbdg(int key,int keytype) 
{
	map<int, Bridge*>::iterator pr;
	pthread_rwlock_rdlock(&mylock);
	if(keytype == KEY_ID)
	{
		pr = id_map.find(key);
		if( pr != id_map.end() )
		{
			pthread_rwlock_unlock(&mylock);
			return pr->second;
		}
	}
	else if(keytype == KEY_SOCK)
	{
		pr = sock_map.find(key);
		if( pr != sock_map.end() )
		{
			pthread_rwlock_unlock(&mylock);
			return pr->second;
		}
	}
	pthread_rwlock_unlock(&mylock);
	return NULL;
}

int BdgmgerClass::addbdg(Bridge* bdg)
{
	assert(bdg != NULL);
	map<int, Bridge*>::iterator pr;

	pr = id_map.find(bdg->id());
	if( pr != id_map.end() )
		return EXIST;

	pr = sock_map.find(bdg->socket());
	if( pr != sock_map.end() )
		return OTHES;

	pthread_rwlock_wrlock(&mylock);
	id_map.insert(pair<int,Bridge*>(bdg->id(),bdg));
	sock_map.insert(pair<int ,Bridge* >(bdg->socket(),bdg));
	pthread_rwlock_unlock(&mylock);
	return SUCCESS;
}

int BdgmgerClass::erasebdg(int key,int keytype,int flag)
{
	map<int, Bridge*>::iterator pr;
	Bridge* bdg;
	pthread_rwlock_wrlock(&mylock);
	if( keytype == KEY_SOCK )
	{
		pr = sock_map.find(key);
		if( pr == sock_map.end() )
		{
			pthread_rwlock_unlock(&mylock);
			return NOEXIST;
		}
		bdg = pr->second;
		sock_map.erase(pr);

		id_map.erase(bdg->id());
		if(flag==FLAG_DEL)
			delete bdg;
	}
	else if(keytype == KEY_ID)
	{
		pr = id_map.find(key);
		if( pr == id_map.end() )
		{
			pthread_rwlock_unlock(&mylock);
			return NOEXIST;
		}
		bdg = pr->second;
		id_map.erase(pr);

		sock_map.erase(bdg->socket());
		if(flag==FLAG_DEL)
			delete bdg;
	}
	pthread_rwlock_unlock(&mylock);
	return SUCCESS;
}


int BdgmgerClass::size() 
{
	pthread_rwlock_rdlock(&mylock);
	int s = id_map.size();
	pthread_rwlock_unlock(&mylock);
	return s;
}

BdgmgerClass::BdgmgerClass()
{
	pthread_rwlock_init(&mylock,NULL);
}
BdgmgerClass::~BdgmgerClass()
{
	pthread_rwlock_destroy(&mylock);
}


int BdgmgerClass::livebdg(vector<Bridge*> *v)
{
	map<int, Bridge*>::iterator pr;
	for(pr = sock_map.begin(); pr!=sock_map.end(); pr++)
	{
		if((pr->second)->socket() != Unknow)
			v->push_back(pr->second);
	}
	return v->size();
}