#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	char tmp[1024] = {0};

	int infd = STDIN_FILENO;

	int seek = read(infd,tmp,1024);

	if(seek == -1)
		perror("stdin:");
	*(tmp+seek-1) = 0;
	printf("%s\n", tmp);
	return 0;
}