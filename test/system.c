#include <unistd.h>
#include <errno.h>
int systemx(const char * cmdstring)
{
    pid_t pid;
    int status;
 
	if(cmdstring == NULL)
	{
	    return (1); //如果cmdstring为空，返回非零值，一般为1
	}
	 
	if((pid = fork())<0)
	{
	    status = -1; //fork失败，返回-1
	}
	else if(pid == 0)
	{
	    execl("/system/bin/sh", "sh", "-c", cmdstring, (char *)0);
	    _exit(127); // exec执行失败返回127，注意exec只在失败时才返回现在的进程，成功的话现在的进程就不存在啦~~
	}
	else //父进程
	{
	    while(waitpid(pid, &status, 0) < 0)
	    {
	        if(errno != EINTR)
	        {
	            status = -1; //如果waitpid被信号中断，则返回-1
	            break;
	        }
	    }
	}
    return status; //如果waitpid成功，则返回子进程的返回状态
}


int main(int argc, char const *argv[])
{
	systemx(argv[1]);
	return 0;
}