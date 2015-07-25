#ifndef MYIO_H_
#define MYIO_H_ 

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

using namespace std;

class Myio
{
public:
	static Myio* createio();

	string myin();
	int myout(string out);

	int myerr(string err);
	~Myio();
	
private:
	Myio();
	static Myio* me;
	int outfd;
	int infd;
	int errfd;
};


#endif

