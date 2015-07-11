#include "pool.h"

int mainlist()
{
	Socket sock;
	if(!sock.create_socket())
		return 0;
	if(!sock.bind_socket(CONST_PORT))
		return 0;
	if(!sock.listen_socket())
		return 0;
	int newfd = 0;
	while(1)
	{
		Address * addrbuf = new Address;
		newfd = sock.accept_socket(addrbuf);
		if(newfd == -1)
		{
			delete addrbuf;
			continue;
		}
		addrbuf->socketfd = newfd;
		Pthread_x pth;
		pth.run(guard,addrbuf);
	}
}

void* guard(void* argument)
{
	Address* addr = (Address*)argument;
	addr->rinit();

	Message msgclass;
	if(msgclass.recvmsg(addr)==-1)
	{
		delete addr;
		return NULL;
	}
	switch( msgclass.getId() )                         //need to some care
	{
		case ID_SHELL:
			break;
		default:
			break;
	}
	delete addr;
}


void* forward(void* argument)
{
	string * command = (string *)argument;

}


void* communicate(void * argument)
{
	string she;
	while(1)
	{
		cout<<"-1-  translate files\n";
		cout<<"-2-  execute program\n";
		cout<<"Your Choose :";
		cin>>she;
		if(she == "1")
		{

		}
		else if(she == "2" )
		{

		}
		else
		{
			cout<<"choose illegal !\n";
		}
	}
}


