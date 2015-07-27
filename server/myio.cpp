#include "myio.h"


Myio* Myio::me = NULL;


Myio::Myio()
{
	infd = STDIN_FILENO;
	outfd = STDOUT_FILENO;
	errfd = STDERR_FILENO;
}

Myio* Myio::createio()
{
	if(me == NULL)
		me = new Myio;
	return me;
}

string Myio::myin()
{
	//
	myout("\nkevin@: ");

	//
	char tmp[1024] = {0};
	int seek = read(infd,tmp,1024);
	*(tmp+seek-1) = 0;
	string k = tmp;
	return k;
}

int Myio::myout(string out)
{
	return write(outfd,out.c_str(),out.size());
	 
}


int Myio::myerr(string err)
{
	return write(errfd,err.c_str(),err.size());
}