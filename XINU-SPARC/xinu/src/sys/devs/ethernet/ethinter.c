/* ethinter.c - ethinter */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <lereg.h>
#include <vmem.h>
#include <q.h>

/*#define DEBUG*/
#define PRINTERRORS

/*------------------------------------------------------------------------
 *  ethinter - ethernet interrupt processing procedure
 *             ** NOTE ** Assumes interrupt is disabled.
 *------------------------------------------------------------------------
 */
INTPROC	ethinter(etptr)
     struct	etblk	*etptr;
{
    STATWORD	ps;
    register    struct le_device *le;
    struct	le_md	*pmd;
    struct	bre	*pbre;
    struct	netif *pni;
    short	csr;

    etptr = &eth[0];
    le = etptr->etle;
    pni = &nif[etptr->etintf];
    
#ifdef DEBUG
    kprintf("\n\t**ethinter: called (nif = %d)\n", etptr->etintf);
#endif
    le->le_rap = LE_CSR0;	/* specify register 0			*/
    csr = le->le_rdp;		/* grab a copy of csr0			*/
    csr &= ~LE_INEA;    /* clear the INEA bit in the copy       */
    le->le_rdp = csr;   /* write the copy back                  */
    le->le_rdp = LE_INEA;       /* turn interrupts back on              */
    /* now, don't touch the register again.  Interrupts are clear       */
    /* (probably).  Anything that happens from now on, we'll catch      */
    /* after we return from this routine.                               */
    
    /* see if a hardware error has occurred */
    if ((csr & LE_ERR) != 0) {
	if (csr & LE_CERR) {
#ifdef PRINTERRORS
	    if (csr&LE_BABL)
		kprintf("LEerr: Babbling xmitter\n");
	    if (csr&LE_CERR)
		kprintf("LEerr: Collision error\n");
 	    if (csr&LE_MISS)
		kprintf("LEerr: Missed packet\n");
#endif	PRINTERRORS
	    if (csr&LE_BABL)
		pni->ni_oerrors++;
	    else
		pni->ni_ierrors++;
	}
	/* reset the interrupt */
	le->le_csr = LE_BABL|LE_CERR|LE_MISS|LE_MERR|LE_INEA;
    }

    /* service any xmit interrupts */
    if ((csr & LE_TINT) != 0) {
#ifdef DEBUG
        kprintf("inside the transmit checking section\n");
#endif
	pbre = &etptr->etbrt[0];
	pmd  = pbre->pmd;
	if ((pmd->lmd_flags & LMD_ERR) == 0) {
#ifdef DEBUG
	    kprintf("*** ethinter: packet xmit successfully\n");
#endif
	    freebuf(etptr->etwbuf);
	    etptr->etoutqlen--;		/* set in ethwstrt() */
	} else if (etptr->etwtry-- > 0) { 	/* retry on error */
#ifdef PRINTERRORS
	    kprintf("LEerr: retrying write(retry:%d)\n", etptr->etwtry);
#endif	
	    /* reset */
	    pmd->lmd_bcnt  = 0;
            pmd->lmd_flags = LMD_OWN | LMD_STP | LMD_ENP;
            le->le_csr = LE_TDMD | LE_INEA;
	} else {
#ifdef PRINTERRORS
	    kprintf("ethinter: too many retries, dropping\n");
#endif
	    freebuf(etptr->etwbuf);
	    etptr->etoutqlen--;
	}
	
	/* check TX queue */
	if (etptr->etoutqlen)
	    ethwstrt(etptr);	
    }
    
    /* check to see if a receive has completed */
    if ((csr & LE_RINT) != 0) {
	/* the chip won't tell us which buffer it filled in */
	/* But, etnextbuf always points to the next buf after */
	/* the last buffer we examined */
#ifdef DEBUG
	kprintf("inside the received checking section\n");
#endif
	if (etptr->etrpending)	/* we have not finished serving it yet */
	    return;
	etptr->etrpending++;
	
	pbre = &etptr->etbrr[etptr->etnextbuf];
	pmd  = pbre->pmd;
	while ((pmd->lmd_flags & LMD_OWN) == 0) {
#ifdef DEBUG
	kprintf("next buf to check is %d\n", etptr->etnextbuf);
#endif
	if (etptr->etrpending)	/* we have not finished
	    /* receive packets */
	    /* We may have a context switch here. That means another  */
	    /* interrupt may occur before we have finished receiving the */
	    /* current packet. That's why we have a "etrpending" flag. */
	    /* --lin  */
	    ethrint(etptr, pmd, pbre);	

	    pmd->lmd_flags = LMD_OWN;	/* LANCE owns it now */
	    pmd->lmd_mcnt  = 0;
	    etptr->etnextbuf = (etptr->etnextbuf + 1) % EP_NUMRCV;
	    pbre = &etptr->etbrr[etptr->etnextbuf];
	    pmd  = pbre->pmd;
	}

	etptr->etrpending--;
	send(ippid, etptr->etintf);
    }

#ifdef DEBUG
    kprintf("\tabout to exit ethinter()\n\n");
#endif
}

/*------------------------------------------------------------------------
 *  ethrint - handle a pep recieved from the ethernet
 *------------------------------------------------------------------------
 */
static ethrint(etptr, pmd, pbre)
     struct	etblk	*etptr;
     struct	le_md	*pmd;
     struct	bre	*pbre;
{
    struct	ep	*pep;
    struct	ep	*pepCpu;
    int	readlen;
    int	errs;
    int	ifnum;

    ifnum = etptr->etintf;

    /* check for receive error */
    if ((pmd->lmd_flags & LMD_ERR) != 0) {
#ifdef PRINTERRORS
	errs = pmd->lmd_flags & (RMD_FRAM|RMD_OFLO|RMD_CRC|RMD_BUFF);
	if (errs != 0) {
	    if (errs & RMD_FRAM)
		kprintf("ethinter: framing error\n");
	    if (errs & RMD_OFLO)
		kprintf("ethinter: buffer overflow\n");
	    if (errs & RMD_CRC)
		kprintf("ethinter: CRC error\n");
	    if (errs & RMD_BUFF)
		kprintf("ethinter: don't own next buffer\n");
	}
#endif PRINTERRORS
	return(SYSERR);
    }
    
    /* check for illegal ifnum (not initialized yet) */
    if ((ifnum <= NI_LOCAL) || (ifnum >= Net.nif)) {
	return SYSERR;
    }
    
    /* check for 'packet too long' */
    readlen = pmd->lmd_mcnt - EP_CRC;
    if (readlen > (EP_DLEN+14)) {
	nif[ifnum].ni_ierrors++;
	return SYSERR;
    }
    
    if (nif[ifnum].ni_state != NIS_UP) {
	nif[ifnum].ni_idiscard++;
	return SYSERR;
    }
    
    pep = (struct ep *) pbre->buf;
    /* ... else no error */
    pepCpu = (struct ep *) getbuf(Net.netpool);
    if (pepCpu == (struct ep *) SYSERR) {
	if (ifnum >= 0 && ifnum < NIF)
	    nif[ifnum].ni_idiscard++;
	return(SYSERR);
    }

    /* give the new buffer to LANCE */
    pbre->buf = (char *) pepCpu;
    pmd->lmd_ladr = low16(LANCE_ADDR((char *)&pepCpu->ep_eh));
    pmd->lmd_hadr =   hi8(LANCE_ADDR((char *)&pepCpu->ep_eh));

    /* get the packet */
    pepCpu = pep;
    pepCpu->ep_type = net2hs(pepCpu->ep_type);
    pepCpu->ep_len = readlen;
    /* pass the pep to the demux procedure */
    (void) ethdemux(etptr, pepCpu, readlen);

#ifdef DEBUG
    kprintf("ethrint: exits\n");
#endif

    return(OK);
}
