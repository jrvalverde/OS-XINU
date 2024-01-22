/* getdtablesize.c - getdtablesize */

#include <kernel.h>

getdtablesize()
{
    return(sys_getdtablesize() - 1);
}
