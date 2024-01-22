/* rpcminit.c - rpcminit */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <mark.h>
#include <rpc.h>

rpcminit()
{
	rpcmutex = screate(1);
	
	return(OK);
}

#ifdef	Nrpc
struct	rpcblk	rpctab[Nrpc];		/* rpc device control blocks	*/
#endif
int	rpcmutex;
MARKER  rpcmark;
