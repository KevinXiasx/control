#ifndef POOL_H_
#define POOL_H_ 

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
	sockfd->rinit();

	Message msgclass;
	if(msgclass.recvmsg(addr)==-1)
	{
		delete sockfd;
		return ;
	}
	switch( msgclass.getId() )
	{
		case:ID_SHELL
			break;
		default:
			break;
	}
	delete sockfd;
}



#endif