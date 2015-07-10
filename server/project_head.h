#ifndef PUBLIC_HEAD_
#define PUBLIC_HEAD_

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <sys/types.h>
#include <string.h>

#endif


#ifndef ERR_OPERAT_H_
#define ERR_OPERAT_H_

//------------------------------------------err produce-----------------------------------------
#define err_return false
#define err_nreturn true

#define ERR(x,y,z,n)                        \
            do {                                               \
                if (x == y) {                               \
                    int save_errno = errno;                \
                    fprintf(stderr, "file:%s,fun:%s(),line:%d,",            \
                            __FILE__, __FUNCTION__, __LINE__);       \
                    errno = save_errno;               \
                    perror(z);                     \
                    fflush(stderr);                         \
                    errno = save_errno;                     \
                    if(n != err_nreturn)     \
                    {               \
                        return n;       \
                    }               \
               }                                                \
            } while (0)

#define DEBUG(x)                \
            do{                     \
                if(sizeof((x))==sizeof(int))      \
                    printf(""#x" = %d\n", (x));      \
                else                              \
                    printf(""#x" = %s\n", (x));      \
            }while(0)   

#endif 

//-----------------------------------------message define------------------------------------

#ifndef MSG_H_
#define MSG_H_ 

#define ID_SHELL 1
#define ID_TFILE 2
#define ID_

typedef union u_msg{
    unsigned id;
    struct {
        unsigned id;
        unsigned shellen;
    } shell_m;
    struct {
        unsigned id;
        unsigned mode;
        unsigned path;
        unsigned size;
    } cfile_m;
}U_MSG;

#endif

