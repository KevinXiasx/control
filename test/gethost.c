#include <netdb.h>
#include <stdio.h>



int main(int argc, char const *argv[])
{
	void* res = gethostbyname(argv[1]);
	printf("%d\n",res == NULL?1:2);
	return 0;
}