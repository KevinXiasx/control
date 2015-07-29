#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define T_HEART 0
#define T_ANSWR 1
#define T_SHELL 2
#define T_TFILE 3

typedef union u_msg{
    unsigned id;
    unsigned type;
    struct 
    {
        unsigned id;
        unsigned type;
    }heart_m;

    struct 
    {
        unsigned id;
        unsigned type;
        unsigned torf;
        unsigned errlen;
    } answer_m;

    struct {
        unsigned id;
        unsigned type;
    } shell_m;

    struct {
        unsigned id;
        unsigned type;
        unsigned mode;
        unsigned path;
        unsigned size;
    } tfile_m;

}U_MSG;


int main(int argc, char const *argv[])
{
    U_MSG k ;
    k.shell_m.id = 1;
    U_MSG * buf = (U_MSG*)malloc(sizeof(U_MSG));
    memcpy(buf,&k,sizeof(U_MSG));
    printf("%d\n", buf->tfile_m.id);
    printf("%d\n", buf->heart_m.id);
    printf("%d\n", buf->id);
    return 0;
}