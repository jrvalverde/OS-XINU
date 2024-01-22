/* ni_in.c - ni_in */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*#define DEBUG*/
/*#define DUMPPKT*/

/*------------------------------------------------------------------------
 *  ni_in - network interface input function
 *------------------------------------------------------------------------
 */
int ni_in(pni, pep, len)
     struct	netif	*pni;		/* the interface	*/
     struct	ep	*pep;		/* the packet		*/
     int		len;		/* length, in octets	*/
{
    int	rv;
    
#ifdef	DEBUG
    kprintf("ni_in: passing up packet 0x%08x, type: 0x%04x\n",
	    pep, pep->ep_type); 
#endif	DEBUG
#ifdef	DUMPPKT
    {
	int j = len;
	int i;
	
	kprintf("----\n");
	while (j > 0) {
	    if (j > 16) {
		for (i = 0; i<16; i++) {
		    kprintf("%02x  ",((char *)pep)[i+(len-j)] & 0xff);
		}
		j -= 16;
	    }
	    else {
		for (i = 0; i<j; i++) {
		    kprintf("%02x  ",((char *)pep)[i+(len-j)] & 0xff);
		}
		j = 0;
	    }
	    kprintf("\n");
	}
	kprintf("----\n");
    }
#endif
    switch (pep->ep_type) {
      case EPT_ARP:
#ifdef DEBUG
	kprintf("ni_in: ARP...\n");
#endif
	rv = arp_in(pni, pep);
	break;
      case EPT_RARP:
#ifdef DEBUG
	kprintf("ni_in: RARP...\n");
#endif
	rv = rarp_in(pni, pep);
	break;
      case EPT_IP:
#ifdef DEBUG
	kprintf("ni_in: IP...\n");
#endif
	rv = ip_in(pni, pep);
	break;
      default:
#ifdef DEBUG
	kprintf("ni_in: UNKNOWN ep_type (%d)...\n", pep->ep_type);
#endif
	pni->ni_iunkproto++;
	freebuf(pep);
	return OK;
    }
    pni->ni_ioctets += len;
    if (blkequ(pni->ni_hwa.ha_addr, pep->ep_dst, EP_ALEN))
	pni->ni_iucast++;
    else
	pni->ni_inucast++;
    return rv;
}
