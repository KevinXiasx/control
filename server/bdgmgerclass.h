#ifndef BRIDGE_H_
#define BRIDGE_H_ 

#include "project_head.h"
#include <map>
#include <event.h>

class EventClass;

typedef int SOCK;
typedef int ID ;
#define Unknow -1

#define FLAG_READ 1
#define FLAG_WRITE 2
#define FLAG_BOTH 3
#define FLAG_DEL 4
#define FLAG_NODEL 5

class Bridge
{
public:
	Bridge();
	Bridge(SOCK sock);
	Bridge(SOCK sock, ID id);
	Bridge(SOCK sock, EventClass * eventmg, Callback backfun, int flag, void* argument=NULL);
	~Bridge();

	ID setid(ID id);
	SOCK socket() const;
	ID id() const;
	int intoevt(EventClass * eventmg, Callback backfun, int flag, void* argument=NULL);
	int outevt(int flag);
	int close();

private:
	SOCK mysock;
	ID myid;
	struct event * myrdevt;
	struct event * mywtevt;
	EventClass * mybase;
};


#define KEY_ID 1
#define KEY_SOCK 2

class BdgmgerClass
{
public:
	BdgmgerClass();
	~BdgmgerClass();

	Bridge* getbdg(int key,int keytype) ;

	int addbdg(Bridge* bdg);
	int erasebdg(int key,int keytype,int flag);

	int livebdg(vector<Bridge*> *v);

	int size() ;

private:
	map<int, Bridge*> id_map;
	map<int, Bridge*> sock_map;
	pthread_rwlock_t mylock;
};


#endif


