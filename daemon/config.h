#ifndef CONFIG_H_
#define CONFIG_H_ 

#include <fcntl.h>
#include "project_head.h"

int readcfg(const char* cfg, char*  value);

int writecfg(const char* cfg, const char * value );




#endif