#include "globalDate.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include "xsystem.h"

using namespace std;


extern Configx myConfig;//配置类

GlobalDate* GlobalDate::myglobalpr = NULL;

GlobalDate* GlobalDate::create()
{
	if(myglobalpr == NULL)
		myglobalpr = new GlobalDate;
	return myglobalpr;
}

GlobalDate::GlobalDate()
{
	writeMem.shareinit(myConfig.getconfint("shmftoakey"),myConfig.getconfint("blocknumb"));
	readMem.shareinit(myConfig.getconfint("shmatofkey"),myConfig.getconfint("blocknumb"));

	requestMem.shareinit(myConfig.getconfint("requestkey"),5);
	answerMem.shareinit(myConfig.getconfint("answerkey"),5);
}
GlobalDate::~GlobalDate()
{
	
}

