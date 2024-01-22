/* ripin.c - rip */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <proc.h>


Bool	dorip = FALSE;
int	rippid = BADPID;
int	riplock;

#define DEBUG

/*------------------------------------------------------------------------
 *  rip  -  do the RIP route exchange protocol
 *------------------------------------------------------------------------
 */
PROCESS rip()
{
    struct	xgram	*pripbuf;
    struct	rip	*prip;
    int		fd, len;
    
    pripbuf = (struct xgram *) getmem(sizeof(struct xgram));
    
    fd = open(UDP, ANYFPORT, UP_RIP);
    if (fd == SYSERR)
	panic("rip: cannot open rip port");
    
    riplock = screate(1);
    if (gateway) {
#ifdef DEBUG
	kprintf("rip: starting ripout process\n");
#endif
	resume(kcreate(ripout, RIPSTK, RIPPRI, RIPONAM, RIPOARGC));
    }
    
    while (TRUE) {
	len = read(fd, pripbuf, sizeof(*pripbuf));
	if (len == SYSERR)
	    continue;
	
	prip = (struct rip *)pripbuf->xg_data;
	if (ripcheck(prip, len) != OK)
	    continue;
	
	switch (prip->rip_cmd) {
	  case RIP_RESPONSE:
	    if (pripbuf->xg_fport == UP_RIP)
		riprecv(prip, len, pripbuf->xg_fip);
	    break;
	  case RIP_REQUEST:
	    if (gateway || pripbuf->xg_fport != UP_RIP)
		riprepl(prip, len, pripbuf->xg_fip,
			pripbuf->xg_fport);
	    break;
	  default:
	    break;
	}
    }
}
