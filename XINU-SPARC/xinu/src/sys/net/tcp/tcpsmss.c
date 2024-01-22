/* tcpsmss.c - tcpsmss */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcpsmss - set sender MSS from option in incoming segment
 *------------------------------------------------------------------------
 */
int tcpsmss(ptcb, ptcp, popt)
     struct	tcb	*ptcb;
     struct	tcp	*ptcp;
     char		*popt;
{
    unsigned		mss, len;
    unsigned int	opt_l;
    unsigned int	opt_s;
    
    len = *++popt;	
    if ((ptcp->tcp_code & TCPF_SYN) == 0)
	return len;
    
    switch (len-2) {	/* subtract kind & len	*/
      case sizeof(char):
	  mss = *popt;
	  break;
	case sizeof(short):
	    blkcopy(&opt_s, popt, sizeof(short));
	  mss = net2hs(opt_s);
	  break;
	case sizeof(long):
	    blkcopy(&opt_l, popt, sizeof(long));
	  mss = net2hl(opt_l);
	  break;
	default:
	  mss = ptcb->tcb_smss;
	  break;
      }
    mss -= TCPMHLEN;	/* save just the data buffer size */
    
    if (ptcb->tcb_smss)
	ptcb->tcb_smss = min(mss, ptcb->tcb_smss);
    else
	ptcb->tcb_smss = mss;
    return len;
}
