/* ee_demux.c - ee_demux */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <ee.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 * ee_demux - demultiplex an input packet from an Intel EtherExpress
 *------------------------------------------------------------------------
 */
int ee_demux(ped)
struct etdev	*ped;
{
    struct ep	*pep, *ee_get();
    unsigned short iobase, rfd_status, rbd, scb_status, tmp;
    int		ifnum;

    iobase = ped->ed_pdev->dvcsr;
    
    kprintf("ee_demux\n");
#ifdef DEBUG
    kprintf("ee_demux\n");
#endif
    
    ifnum = ped->ed_ifnum;
    if (ifnum < 0 || ifnum >= NIF) {
	kprintf("** ee_demux: bad ifnum=%d\n", ifnum);
	return;
    }

    outw(iobase+RDPTR, ped->ed_rfd);
    rfd_status = inw(iobase);	/* status */
    tmp = inw(iobase);		/* command */
    tmp = inw(iobase);		/* link */
    rbd = inw(iobase);		/* rbd pointer */
    
    while (rfd_status & RFD_C) {
	if (nif[ifnum].ni_state == NIS_UP)
	    pep = ee_get(ped, iobase, rfd_status, rbd);	/* get frame */
	else
	    pep = 0;

	/*
	 * put the just received frame descriptor at the end
	 */
	outw(iobase+WRPTR, ped->ed_rfd);
	outw(iobase, 0);		/* status	*/
	outw(iobase, RFD_EL);		/* end of list	*/
	
	/*
	 * modify the (previous) last frame descriptor
	 */
	outw(iobase+WRPTR, ped->ed_rfdend+2);
	outw(iobase, 0);		/* command	*/

	/*
	 * update END frame pointer
	 */
	ped->ed_rfdend = ped->ed_rfd;
	    
	/*
	 * the next one becomes the head
	 */
	outw(iobase+RDPTR, ped->ed_rfd+4);	/* next rfd */    
	ped->ed_rfd = inw(iobase);

	/*
	 * modify SCB's rfa pointer
	 */
	outw(iobase+SCB_RFA, ped->ed_rfd);
	
	/*
	 * ni_in can cause a context switch, so restart receive
	 * unit, if necessary, here.
	 */
	scb_status = inw(iobase+SCB_STATUS);
	if ((scb_status & SCBSTAT_RUR) == 0) {
	    ee_wait(iobase);
	    outw(iobase+SCB_COMMAND, SCBCMD_RUS);
	    outb(iobase+CAC, 0);
	    ee_wait(iobase);
	    ee_ack(iobase);
	}
	if (pep) {
#ifdef	NETMON
	    nm_in(&nif[ifnum], pep, pep->ep_len);
	    if (!ethmatch(&nif[ifnum], pep->ep_dst, pep->ep_len))
		freebuf(pep);
	    else
#endif	/* NETMON */
		/*
		 * receive packets
		 */
		ni_in(&nif[ifnum], pep, pep->ep_len);
	}

	/*
	 * check next RFD
	 */
	outw(iobase+RDPTR, ped->ed_rfd);
	rfd_status = inw(iobase);	/* status */
    }
}


/*-------------------------------------------------------------------------
 * ee_get - get frame 
 *-------------------------------------------------------------------------
 */
struct ep *ee_get(ped, iobase, rfd_status, rbd)
struct etdev	*ped;
unsigned short iobase, rfd_status, rbd;
{
    struct ep	*pep;
    int		ifnum = ped->ed_ifnum, count, i;
    unsigned short   *p;
    unsigned short rbd_status, rbd_next, rbd_buflo, rbd_bufhi;

#ifdef DEBUG
/*	kprintf("ee_get: rfd_status=%x, rbd=%d\n", rfd_status, rbd);*/
#endif
    if ((rfd_status & RFD_OK) == 0) {
	nif[ifnum].ni_idiscard++;
#ifdef DEBUG
	kprintf("ee_get: recvd bad frame (rfd status=%x)\n", rfd_status);
#endif
	return 0;
    }
    
    pep = (struct ep *)nbgetbuf(Net.netpool);
    if (pep == 0) {
	if (ifnum >= 0 && ifnum < NIF)
	    nif[ifnum].ni_idiscard++;
#ifdef DEBUG
	kprintf("ee_get: no buffer\n");
#endif
	return 0;
    }
    
    /* receive buffer descriptor */
    outw(iobase+RDPTR, rbd);
    rbd_status = inw(iobase);
    rbd_next = inw(iobase);
    rbd_buflo = inw(iobase);
    rbd_bufhi = inw(iobase);
#ifdef DEBUG
/*    kprintf("rbd_status=%x, rbd_next=%x, rbd_buflo=%x, rbd_bufhi=%x\n",*/
/*	   rbd_status, rbd_next, rbd_buflo, rbd_bufhi);*/
#endif
    
    if (!(rbd_status & RBD_VALID)) {
	freebuf(pep);
#ifdef DEBUG
	kprintf("ee_get: invalid frame (rbd_status=%x)\n", rbd_status);
#endif	
	return 0;
    }

    count = rbd_status & RBD_COUNT_MASK;
#ifdef DEBUG
	kprintf("ee_get: rbd count=%d\n", count);
#endif  
    pep->ep_len = count;
    if (pep->ep_len - sizeof(struct eh) > EP_DLEN) {
	kprintf("ee%d: packet too large (%d)\n", pep->ep_len);
	nif[ifnum].ni_idiscard++;
	freebuf(pep);
	return 0;
    }

    /* copy frame from receive buffer */
    outw(iobase+RDPTR, rbd_buflo);
    p = (unsigned short *)&pep->ep_eh;
    for (i = 0; i < (pep->ep_len + 1) >> 1; i++) {
	p[i] = inw(iobase);
    }
	
    pep->ep_type = net2hs(pep->ep_type);
    pep->ep_order = EPO_NET;

#ifdef DEBUG   
    {
	int i;
	
        kprintf("ETHER: dst %02x", pep->ep_dst[0]);
        for (i=1; i<EP_ALEN; ++i)
                kprintf(":%02x", pep->ep_dst[i]);
        kprintf(" src %02x", pep->ep_src[0]);
        for (i=1; i<EP_ALEN; ++i)
                kprintf(":%02x", pep->ep_src[i]);
        kprintf("\nETHER: type %x\n", pep->ep_type);
    }
#endif
    return pep;
}

