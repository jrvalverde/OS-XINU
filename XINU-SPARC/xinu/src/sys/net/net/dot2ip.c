/* dot2ip.c - dot2ip */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  dot2ip - convert a dotted decimal string into an IP address
 *------------------------------------------------------------------------
 */
char *dot2ip(ip, pdot)
     IPaddr	ip;
     char	*pdot;
{
    int	i;
    
    for (i=0; i<IP_ALEN; ++i)
	ip[i] = 0;
    
    for (i=0; i<IP_ALEN; ++i) {
	ip[i] = atoi(pdot);
	if ((pdot = (char *) index(pdot, '.')) == NULL)
	    break;		/* no more dots, everything else is 0 */
	++pdot;
    }
    return ip;
}
