#ifndef ERR_OPERAT_H_
#define ERR_OPERAT_H_


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

//--------------------------------------err produce-----------------------------------------
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
                    if(n == err_return)     \
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



#endif /* PROJECT_HEAD_H_ */
