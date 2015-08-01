#include <string.h>
#include <iostream>

using namespace std;

void rsolvepath(const string& command, string& sourse, string& dst)
{
	string::size_type loc = command.find("/",0);
	string::size_type loc2 = command.find(" ",loc);
	string::size_type loc3 = command.find("/",loc2);

	sourse = command.substr(loc,loc2-loc);
	dst = command.substr(loc3);
}

int main(int argc, char const *argv[])
{
	string s = "/system/kevin to /home/kevin";

	string sour,dst;

	rsolvepath(s,sour,dst);

	cout<<"sour:"<<sour<<endl;
	cout<<"dst:"<<dst<<endl;

}