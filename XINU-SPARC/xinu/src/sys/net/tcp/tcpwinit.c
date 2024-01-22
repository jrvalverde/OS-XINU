/* tcpwinit.c - tcpwinit */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>

/*------------------------------------------------------------------------
 *  tcpwinit - initialize window data for a new connection
 *------------------------------------------------------------------------
 */
int tcpwinit(ptcb, newptcb, pep)
     struct	tcb	*ptcb, *newptcb;
     struct	ep	*pep;
{
    struct	ip	*pip = (struct ip *)pep->ep_data;
    struct	tcp	*ptcp = (struct tcp *)pip->ip_data;
    struct	route	*prt, *rtget();
    Bool		local;
    int		mss;
    
    newptcb->tcb_swindow = ptcp->tcp_window;
    newptcb->tcb_lwseq = ptcp->tcp_seq;
    newptcb->tcb_lwack = newptcb->tcb_iss;	/* set in tcpsync()	*/
    
    prt = (struct route *)rtget(pip->ip_src, RTF_REMOTE);
    local = prt && prt->rt_metric == 0;
    newptcb->tcb_pni = &nif[prt->rt_ifnum];
    rtfree(prt);
    if (local)
	mss = newptcb->tcb_pni->ni_mtu - IPMHLEN - TCPMHLEN;
    else
	mss = 536;	/* RFC 1122 */
    
    if (ptcb->tcb_smss) {
	newptcb->tcb_smss = min(ptcb->tcb_smss, mss);
	ptcb->tcb_smss = 0;			/* reset server smss	*/
    } else
	newptcb->tcb_smss = mss;

    newptcb->tcb_rmss = mss;			/* receive mss		*/
    newptcb->tcb_cwnd = newptcb->tcb_smss;	/* 1 segment		*/
    newptcb->tcb_ssthresh = 65535;		/* IP max window	*/
    newptcb->tcb_rnext = ptcp->tcp_seq;
    newptcb->tcb_cwin = newptcb->tcb_rnext + newptcb->tcb_rbsize;
#ifdef SJCDEBUG
    rec_tcp_seg(0, 1, "TCPWINIT: mss:  %d",  mss);
    rec_tcp_seg(0, 1, "TCPWINIT: rmss: %d", newptcb->tcb_rmss);
    rec_tcp_seg(0, 1, "TCPWINIT: smss: %d", newptcb->tcb_smss);
    rec_tcp_seg(0, 1, "TCPWINIT: cwnd: %d", newptcb->tcb_cwnd);
    rec_tcp_seg(0, 1, "TCPWINIT: cwin: %d", newptcb->tcb_cwin);
#endif		
}
