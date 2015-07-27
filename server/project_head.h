#ifndef PUBLIC_HEAD_
#define PUBLIC_HEAD_

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>


using namespace std;

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
                cout<<"file:"<<__FILE__<<"  line:"<<__LINE__<<endl;  \
            }while(0);          \


#endif 

//-----------------------------------------message define------------------------------------

#ifndef MSG_H_
#define MSG_H_ 

#define ID_HEART 0
#define ID_ANSWR 1
#define ID_SHELL 2
#define ID_TFILE 3
#define ID_

typedef union u_msg{
    unsigned id;
    struct 
    {
        unsigned id;
        unsigned torf;
        unsigned errlen;
    } answer_m;

    struct {
        unsigned id;
    } shell_m;

    struct {
        unsigned id;
        unsigned mode;
        unsigned path;
        unsigned size;
    } tfile_m;

}U_MSG;


#endif

