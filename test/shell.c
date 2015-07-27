#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	int red = mkdir("/bin",777);
	if(red == -1)
		perror("err:");
	red = symlink("/system/bin/sh","/bin/sh");
	if(red == -1)
		perror("err:");

	system(argv[1]);

	printf("over\n");
	return 0;
}