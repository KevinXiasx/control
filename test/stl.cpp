#include <vector>

#include <string.h>
#include <list>
#include <mysql.h>
#include <iostream>
#include <my_global.h>


using namespace std;

int main(int argc, char const *argv[])
{
	std::list<string> v;
	string a1 = "i`m a1";

	v.push_back(a1);
	cout<<*(v.begin())<<endl;
	return 0;
}