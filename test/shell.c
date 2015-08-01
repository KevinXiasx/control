#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int r_creatfile(char * path)
{
	int i=  0;
	for (; i < 2; ++i)
	{
		int red = open(path,O_RDWR|O_CREAT|O_TRUNC,0777);
		if(red == -1)
		{
			if( errno != ENOENT)
				return -1;
			char * pr = strrchr(path,'/');
			if(pr == path)
				return 0;
			char newpath[50] = {0};
			memcpy(newpath, path, pr-path);
			printf("%s\n", newpath);
			r_mkdir(newpath);
		}
		else
			return red;
	}
	return 0;	
}


int r_mkdir(char * path)
{
	int i=  0;
	for (; i < 2; ++i)
	{
		int red = mkdir(path,0777);
		if(red == -1)
		{
			if( errno != ENOENT)
				return -1;
			char * pr = strrchr(path,'/');
			if(pr == path)
				return 0;
			char newpath[50] = {0};
			memcpy(newpath, path, pr-path);
			printf("%s\n", newpath);
			r_mkdir(newpath);
		}
		else
			return 0;
	}
	return 0;
}


int main(int argc, char const *argv[])
{
	int red = r_creatfile("/home/kevin/project_c/control/test/haha/meiyou/heeh");
	if(red == -1)
	{
		perror("err:");
	}
/*	red = symlink("/system/bin/sh","/bin/sh");
	if(red == -1)
		perror("err:");

	system(argv[1]);*/

	printf("over\n");
	return 0;
}