#include "myio.h"



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
	char tmp[1024] = {0};
	read(infd,tmp,1024);
	string k = tmp;
	return k;
}

int Myio::myout(string out)
{
	return write(outfd,out.c_str(),out.size());
	 
}

int Myio::myout(char * out)
{
	return write(outfd,out,strlen(out));
}


int Myio::myerr(string err)
{
	return write(errfd,err.c_str(),err.size());
}