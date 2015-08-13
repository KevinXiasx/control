#include "../include/configx.h"
#include <string.h>

#define CFGNUM  4
static char * cfgname[CFGNUM] = {"ip","stocport","ctosport","id"};
static char * cfgvalue[CFGNUM] = {"sz.radxa.info","1912","1122","0"};

int readcfg(const char* cfg, char*  value)
{
	int n = 0 ;
	int fd = open("/etc/con.cfg",O_RDWR|O_CREAT|O_EXCL,0644);
	if(fd == -1)
	{
		if(errno == EEXIST)
		{
			int fd2 = open("/etc/con.cfg",O_RDONLY);
			if(fd2 < 0){
				perror("open err");
				wrcfg("all", NULL);
				goto label;
			}

			char buff[1024];
			int res = read(fd2,buff,1024);
			if(res <= 0)
				goto label;
			char * pr1 = strstr(buff,cfg);
			if(pr1 == NULL){
				wrcfg(cfg, NULL);
				goto label;
			}

			pr1 = pr1+strlen(cfg)+1;
			char * pr2 = strstr(pr1,"\n");
			*pr2 = 0;
			strcpy(value,pr1);
			close(fd2);
			return true;
		}
		else
			return false;
	}
	close(fd);
	wrcfg("all", NULL);
label:
	for( ; n<CFGNUM; n++)
	{
		if(strcmp(cfg,cfgname[n]) == 0)
		{
			strcpy(value,cfgvalue[n]);
			return true;
		}
	}
	return false;
}

int wrcfg(const char* cfg, char* value)
{
	if( strcmp(cfg, "all") == 0)
	{
		int loop, fd;
		if((fd = open("/etc/con.cfg", O_RDWR|O_CREAT|O_TRUNC, 0644)) == -1)
			return false;

		for(loop=0; loop<CFGNUM; loop++){
			char keyval[30] = {0};
			sprintf(keyval, "%s=%s\n", cfgname[loop], cfgvalue[loop]);
			write(fd, keyval, strlen(keyval));
		}
	}
	else
	{
		int loop1, loop2, fd, ret;
		char *dstpr, *oldpr;
		if((fd = open("/etc/con.cfg",O_RDWR)) == -1)
			return false;

		char buff[1024] = {0};
		if( (ret = read(fd, buff, 1024)) <= 0)
			return false;

		close(fd);
		if( (fd = open("/etc/con.cfg", O_RDWR|O_CREAT|O_TRUNC, 0644)) == 0)
			return false;

		if( (dstpr = strstr(buff, cfg)) == NULL)
			return false;
		dstpr = dstpr+strlen(cfg)+1;
		oldpr = strchr(dstpr, '\n');

		int n = write(fd, buff, dstpr - buff);
		if(n<=0)
			perror("wirte err");
		if(value == NULL){
			for(loop1 = 0 ; loop1<CFGNUM; loop1++)
				if(strcmp(cfg,cfgname[loop1]) == 0)
					write(fd, cfgvalue[loop1], strlen(cfgvalue[loop1]));
		}
		else{
			n = write(fd, value, strlen(value));
			if(n<=0)
			perror("wirte err");
		}
		n = write(fd, oldpr, ret - (oldpr - buff));
		if(n<=0)
			perror("wirte err");
		close(fd);
	}
}

