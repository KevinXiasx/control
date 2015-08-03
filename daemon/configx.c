#include "configx.h"
#include <string.h>

int readcfg(const char* cfg, char*  value)
{

#define CFGNUM  3

	char * cfgname[CFGNUM] = {"ip","stocport","ctosport"};
	char * cfgvalue[CFGNUM] = {"sz.radxa.info","1912","1122"};
	DEBUGW;
	int fd = open("/etc/con.cfg",O_RDWR|O_CREAT|O_EXCL,0644);
	if(fd == -1)
	{
		if(errno == EEXIST)
		{
			int fd2 = open("/etc/con.cfg",O_RDONLY);
			if(fd2 <= 0)
				goto label;

			char buff[1024];
			int res = read(fd2,buff,1024);

			char * pr1 = strstr(buff,cfg);
			if(pr1 == NULL)
				goto label;

			pr1 = pr1+strlen(cfg)+1;
			char * pr2 = strstr(pr1,"\n");
			*pr2 = 0;
			strcpy(value,pr1);
			if(res <= 0)
				goto label;
			return true;
		}
		else
		{
			return false;
		}
	}

	close(fd);
	
label:
	DEBUGW;
	int n = 0 ;
	for( ; n<CFGNUM; n++)
	{
		DEBUGS(cfgname[n]);
		if(strcmp(cfg,cfgname[n]) == 0)
		{
			DEBUGW;
			DEBUGS(cfgvalue[n]);
			strcpy(value,cfgvalue[n]);

			return true;

		}
	}
	return false;
	
}

int writecfg(const char* cfg, const char * value )
{

}


