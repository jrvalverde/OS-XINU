/* getaddr.c - getaddr, getiaddr */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  getaddr - obtain this hosts's primary IP address
 *------------------------------------------------------------------------
 */
SYSCALL getaddr(ip)
     IPaddr	ip;
{
    return getiaddr(NI_PRIMARY, ip);
}

/*------------------------------------------------------------------------
 *  getiaddr  -  obtain this interface's complete address (IP address)
 *------------------------------------------------------------------------
 */
int getiaddr(inum, ip)
     int	inum;
     IPaddr	ip;
{
    struct	netif *intf;
    
    if (inum == NI_LOCAL || inum < 0 || inum >= Net.nif)
	return SYSERR;
    
    intf = &nif[inum];
    wait (rarpsem);
    if (!intf->ni_ivalid && intf->ni_state == NIS_UP) {
	rarpsend(inum);
    }
    signal(rarpsem);
    
    if (!intf->ni_ivalid)
	return SYSERR;
    
    IP_COPYADDR(ip, intf->ni_ip);
    return OK;
}
