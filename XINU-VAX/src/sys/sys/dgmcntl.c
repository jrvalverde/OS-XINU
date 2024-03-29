/* dgmcntl.c - dgmcntl */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  dgmcntl  -  control the network and datagram interface pseudo devices
 *------------------------------------------------------------------------
 */
dgmcntl(devptr, func, addr)
struct	devsw	*devptr;
int func;
char *addr;
{
	int	status = OK;

	switch ( func )	{
#ifdef	NETDAEMON
	case NC_SETGW:
		blkcopy(Net.gateway, addr, IPLEN);
		break;
#endif
	default:
		status = SYSERR;
	}
	return(status);
}
