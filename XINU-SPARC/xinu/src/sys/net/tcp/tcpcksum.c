/* tcpcksum.c - tcpcksum */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcpcksum -  compute a TCP pseudo-header checksum
 *------------------------------------------------------------------------
 */
unsigned short tcpcksum(pip)
     struct	ip	*pip;
{
    struct	tcp	*ptcp = (struct tcp *)pip->ip_data;
    unsigned	short	*sptr;
    unsigned	long tcksum;
    int		len, i;
    
    tcksum = 0;
    
    sptr = (unsigned short *) pip->ip_src;
    /* 2*IP_ALEN octets = IP_ALEN shorts... */
    /* they are in net order.		*/
    for (i=0; i<IP_ALEN; ++i)
	tcksum += *sptr++;
    sptr = (unsigned short *)ptcp;
    len = pip->ip_len - IP_HLEN(pip);	/* TCP length in octet */
    tcksum += hs2net(IPT_TCP + len);
    if (len % 2) {
	((char *)ptcp)[len] = 0;	/* pad */
	len += 1;	/* for the following division */
    }
    len >>= 1;	/* convert to length in shorts */
    
    /* unrolled loop */
    while (len >= 10) {
	tcksum += sptr[0];
	tcksum += sptr[1];
	tcksum += sptr[2];
	tcksum += sptr[3];
	tcksum += sptr[4];
	tcksum += sptr[5];
	tcksum += sptr[6];
	tcksum += sptr[7];
	tcksum += sptr[8];
	tcksum += sptr[9];
	sptr += 10;
	len -= 10;
    }
    
    /* slow loop to clean up */
    while (len > 0) {
	tcksum += *sptr++;
	--len;
    }
    
    tcksum = (tcksum >> 16) + (tcksum & 0xffff);
    tcksum += (tcksum >> 16);
    
    return (short)(~tcksum & 0xffff);
}
