/* tcpok.c - tcpok */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcpok -  determine if a received segment is acceptable
 *------------------------------------------------------------------------
 */
int tcpok(ptcb, pep)
     struct	tcb	*ptcb;
     struct	ep	*pep;
{
    struct	ip	*pip = (struct ip *)pep->ep_data;
    struct	tcp	*ptcp = (struct tcp *) pip->ip_data;
    int		seglen, wlast, slast, rwindow;	/* should be signed */
    Bool	rv;
    
    if (ptcb->tcb_state < TCPS_SYNRCVD)	/* no initial segment sequence yet */
	return TRUE;
    
    seglen = pip->ip_len - IP_HLEN(pip) - TCP_HLEN(ptcp);
    
    /* add SYN and FIN */
    if (ptcp->tcp_code & TCPF_SYN)
	++seglen;
    if (ptcp->tcp_code & TCPF_FIN)
	++seglen;
    
    rwindow = ptcb->tcb_rbsize - ptcb->tcb_rbcount; /* receive window size */
    if (rwindow == 0 && seglen == 0)
	return (ptcp->tcp_seq == ptcb->tcb_rnext);
    
    if (ptcp->tcp_code & TCPF_URG)
	(void) tcprcvurg(ptcb, pep);
    
    wlast = ptcb->tcb_rnext + rwindow - 1;
    rv = SEQCMP(ptcp->tcp_seq, ptcb->tcb_rnext) >= 0 &&
	SEQCMP(ptcp->tcp_seq, wlast) <= 0;
    /*
    rv = (ptcp->tcp_seq - ptcb->tcb_rnext) >= 0 &&
	(ptcp->tcp_seq - wlast) <= 0;*/
  
    if (seglen == 0)
	return rv;
    
    slast = ptcp->tcp_seq + seglen - 1;
    rv |= SEQCMP(slast, ptcb->tcb_rnext) >= 0 && SEQCMP(slast, wlast) <= 0;
    /*rv |= (slast - ptcb->tcb_rnext) >= 0 && (slast - wlast) <= 0; */
    
    /* If no window, strip data but keep ACK, RST and URG		*/
    if (rwindow == 0)
	pip->ip_len = IP_HLEN(pip) + TCP_HLEN(ptcp);
    return rv;
}