#include "packet_x.h"
#include "globalDate.h"
#include "CRC32.h"

PacketClass::PacketClass()
{
	isable = false;
	myid_fd = 0;
	errp = 0;
	packetnumb = 0;
}
PacketClass::PacketClass(int type)
{
	
}

PacketClass::~PacketClass()
{
	
}

st_packet* PacketClass::getPackPr(int i) const
{
	assert(i<packetnumb);
	assert(isable);
	return myPackets[i];
}
int PacketClass::getId() const
{
	assert(isable);
	return myPackets[0]->packethead.idSend;
}

int PacketClass::getPacketNumb() const
{
	return packetnumb;
}
int PacketClass::getType() const
{
	assert(isable);
	return myPackets[0]->packetstat.packetType;
}

int PacketClass::getTrans() const
{
	return myPackets[0]->packetstat.idTrans;
}

int PacketClass::getFd() const
{
	assert(isable);
	assert(myid_fd != 0);
	return myid_fd;
}


void PacketClass::setFd(int fd)
{
	assert(isable);
	myid_fd = fd;
}


int PacketClass::iscomplete(int i) const
{
	assert(i<packetnumb);
	assert(isable);
	if(myPackets[i]->headInt == HEAD_NUMBER)
	{
		return 1;
	}
	else
		return 2;
}


int PacketClass::getPacketSize(int i) const 
{
	assert(i<packetnumb);
	assert(isable);
	int size = sizeof(st_packethead)+sizeof(st_packetstat)+2*sizeof(int)+myPackets[i]->packethead.datasize;
	return size;
}


st_packet* PacketClass::mollocPacket()
{
	isable = true;
	st_packet* k = new st_packet;
	memset(k,0,sizeof(st_packet));
	myPackets.clear();
	packetnumb = 0;
	myPackets.push_back(k);
	packetnumb++;
	return k;
}
void PacketClass::destroyPacket()
{
	static int i = 0;
	assert(isable);
	isable = false;
	std::vector<st_packet*>::iterator pr;
	for(pr = myPackets.begin();pr!=myPackets.end();pr++)
		delete *pr;
	myPackets.clear();
	packetnumb = 0;
}


/*void PacketClass::setType(int type)
{
	static int i = 0;
	i++;
	mollocPacket();
	myPacket->headInt = HEAD_NUMBER;
	
	myPacket->packetstat.packetType = type;
	switch(type)
	{
		case PACKET_BREAKHEARD:
			myPacket->packethead.idSend = 95123;
			myPacket->packethead.numbPacket = 0;
			myPacket->packethead.sumPacket = 1;
			*(myPacket->packethead.CRC) = 0;
			myPacket->packetstat.idTrans = i;
			myPacket->packethead.datasize = 0;
			*(int*)(myPacket->information+myPacket->packethead.datasize) = TAIL_NUMBER;
			break;
		default:
			break;
	}
}*/

void PacketClass::helpclient(int tempid)
{
	mollocPacket();
	myPackets[0]->headInt = HEAD_NUMBER;
	myPackets[0]->packetstat.packetType = PACKET_MAINLIST;

	myPackets[0]->packethead.idSend = tempid;
	myPackets[0]->packethead.numbPacket = 0;
	myPackets[0]->packethead.sumPacket = 1;
	myPackets[0]->packetstat.idTrans = 0;
	myPackets[0]->packethead.datasize = 0;
	unsigned int mycrc;
	StringCRC32((unsigned char * )myPackets[0], &mycrc,0);
	myPackets[0]->packethead.CRC = mycrc;
	*(int*)(myPackets[0]->information+myPackets[0]->packethead.datasize) = TAIL_NUMBER;
}



int PacketClass::checkPacket(int i)
{
	assert(i<packetnumb);
	if(myPackets[i]->headInt == HEAD_NUMBER)//包头正确
	{
		if(myPackets[i]->packethead.datasize <= INFORMATIONMAX)//信息长度正确
		{
			void* pr = myPackets[i]->information+myPackets[i]->packethead.datasize;
			if( *(int*)pr == TAIL_NUMBER )//包尾正确
			{
				if(CheckCRC32(myPackets[i]->packethead.CRC,(unsigned char*)(myPackets[i]->information),myPackets[i]->packethead.datasize))  //此处为CRC校验码确定
				{
					if( *((int*)pr+1) != HEAD_NUMBER)//并未产生断包
					{
						errp = 0;
						return 0;
					}
					else
					{
						errp = 5;
						cout<<"get a err="<<errp<<" packet;"<<endl;
						return 5;
					}
				}
				else
				{
					errp = 4;
					cout<<"get a err="<<errp<<" packet;"<<endl;
					return 4;
				}
			}
			else
			{
				errp = 3;
				cout<<"get a err="<<errp<<" packet;"<<endl;
				return 3;
			}
		}
		else
		{
			errp = 2;
			cout<<"get a err="<<errp<<" packet;"<<endl;
			return 2;
		}
	}
	else
	{
		errp = 1;
		cout<<"get a err="<<errp<<" packet;"<<endl;
		return 1;
	}
}

int PacketClass::getErro() const
{
	return errp;
}


PacketClass PacketClass::operator+(const PacketClass &p) const
{
	PacketClass newp;
	newp.myPackets = this->myPackets;
	newp.packetnumb = this->packetnumb;
	newp.myid_fd = this->myid_fd;

	for (int i = 0; i < p.getPacketNumb(); ++i)
	{
		newp.packetnumb++;
		newp.myPackets.push_back(p.getPackPr(i));
	}
	return newp;
}
bool PacketClass::operator<(const PacketClass &p) const
{
	return (this->myPackets[0]->packethead.numbPacket) < (p.myPackets[0]->packethead.numbPacket);
}

bool PacketClass::islastpart() const
{
	return myPackets[packetnumb-1]->packethead.sumPacket == 1+myPackets[packetnumb-1]->packethead.numbPacket;
}


void PacketClass::answerset(const PacketClass* from)
{
	myPackets[0]->headInt = HEAD_NUMBER;
	myPackets[0]->packethead.idSend = from->getId();
	myPackets[0]->packetstat.packetType = from->getType();
	myPackets[0]->packetstat.idTrans = from->getTrans();
	myPackets[0]->packethead.datasize = 0;
}

void PacketClass::setanswerdata(void* data,int datalen)
{
	if( (myPackets[0]->packethead.datasize + datalen)<= INFORMATIONMAX)//数据量不会唱产生分包
	{
		memcpy(myPackets[0]->information+myPackets[0]->packethead.datasize,data,datalen);
		(myPackets[0]->packethead.datasize)+=datalen;
		*(int*)(myPackets[0]->information+myPackets[0]->packethead.datasize) = TAIL_NUMBER;
		myPackets[0]->packethead.numbPacket = 0; 
		myPackets[0]->packethead.sumPacket = 1;
		unsigned int k;
		StringCRC32((unsigned char*)myPackets[0]->information,&k,myPackets[0]->packethead.datasize);
		myPackets[0]->packethead.CRC = k;
	}
}