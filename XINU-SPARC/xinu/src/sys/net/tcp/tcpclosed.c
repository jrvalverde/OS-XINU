/* tcpclosed.c - tcpclosed */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcpclosed -  do CLOSED state processing
 *------------------------------------------------------------------------
 */
int tcpclosed(ptcb, pep)
     struct	tcb	*ptcb;
     struct	ep	*pep;
{
    tcpreset(pep);	/* RST it */
    return	SYSERR;
}
