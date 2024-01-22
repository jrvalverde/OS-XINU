/* getstk.c - getstk */

#include <kernel.h>
#include <mem.h>

char *malloc();

char *
getstk(nbytes)
int nbytes;
{
    char *tmp;

    tmp = malloc(nbytes);
    return(tmp == NULL ? NULL : tmp + nbytes - sizeof(long));
}
