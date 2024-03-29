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
IPaddr getiaddr(inum)
int	inum;
{
    struct netif	*pif;

    if (inum == NI_LOCAL || inum < 0 || inum >= Net.nif)
	return SYSERR;
    pif = &nif[inum];
    wait (rarpsem);
    if ((!pif->ni_ivalid) && (pif->ni_state == NIS_UP)) {
	rarpsend(inum);
    }
    signal(rarpsem);

    if (!pif->ni_ivalid)
	return SYSERR;

    return pif->ni_ip;
}
