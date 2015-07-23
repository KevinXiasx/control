#include <iostream>

#include <string.h>
using namespace std;


class single
{
public:
	static single* create();
	~single();
private:
	single();
	static single* me;
};

single* single::create()
{
	if(me==NULL)
		me = new single;
	return me;
}

single::single()
{
	me = NULL;
}

single* single::me = NULL;

int main(int argc, char const *argv[])
{
	single* k = single::create();
	if(k == NULL)
		cout<<"null\n";
	else
		cout<<"not null\n";
	return 0;
}