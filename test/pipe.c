#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
	int fd = open("file.txt",O_RDWR,0644);
	int k = dup2(fd,STDIN_FILENO);
	char buf[1024] = {0};
	scanf("%s",buf);
	printf("%s\n", buf);	
	return 0;
}




/*




sock --->  in  -->


*/