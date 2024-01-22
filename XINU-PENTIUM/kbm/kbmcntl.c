/* kbmcntl.c - kbmcntl */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  kbmcntl  -  control a  keyboard/monitor device
 *------------------------------------------------------------------------
 */
kbmcntl(devptr, func, addr)
struct	devsw	*devptr;
int func;
char *addr;
{
    return SYSERR;
}
