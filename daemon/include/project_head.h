#ifndef PUBLIC_HEAD_
#define PUBLIC_HEAD_

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <string.h>

#endif


#ifndef ERR_OPERAT_H_
#define ERR_OPERAT_H_

//------------------------------------------err produce-----------------------------------------
#define false 0
#define true 1


#define err_return -1
#define err_nreturn 0

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

#define DEBUGS(x)                \
            do{                     \
                printf(""#x" = %s\n", (x));      \
            }while(0)   

#define DEBUGI(x)                \
            do{                     \
                printf(""#x" = %d\n", (int)(x));      \
            }while(0)              

#define DEBUGW               \
            do{                 \
                printf("file:%s  line:%d\n", __FILE__,__LINE__);\
            }while(0);          \


#endif 

//-----------------------------------------message define------------------------------------
#ifndef MSG_H_
#define MSG_H_ 

#define T_HEART 0   
#define T_REGST 1   
#define T_ANSWR 2
#define T_SHELL 3
#define T_TFILE 4

typedef union u_msg{
    unsigned type;
    struct 
    {
        unsigned type;
        unsigned id;
    }heart_m;

    struct 
    {
        unsigned type;
        unsigned id;
    }regist_m;
    struct 
    {
        unsigned type;
        unsigned id;
        unsigned torf;
        unsigned loglen;
    } answer_m;

    struct {
        unsigned type;
        unsigned id;
        unsigned commandlen;
    } shell_m;

    struct {
        unsigned type;
        unsigned id;
        unsigned mode;
        unsigned path;
        unsigned size;
    } tfile_m;

}U_MSG;

#endif