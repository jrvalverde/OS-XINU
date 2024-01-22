/* ethinit.c - ethinit */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <lereg.h>
#include <idprom.h>
#include <vmem.h>
#include <q.h>

/*#define DEBUG*/

#ifdef	Neth
struct	etblk	eth[Neth];
#endif

struct idprom idprom;

/*------------------------------------------------------------------------
 *  ethinit  -  initialize ethernet I/O device and buffers
 *------------------------------------------------------------------------
 */
ethinit(devptr)
	struct	devsw	*devptr;
{
    struct	etblk	*etptr;
    struct	le_device *le;
    struct	le_init_block *ib;
    struct	le_md	(*pmdrecv)[];
    struct	le_md	*pmdxmit;
    struct	le_md	*pmd;
    struct	bre	*pbre;
    int		i;
    char 	*pbufr;
    char	*pbuft;
    struct      ep *pep;

#ifdef DEBUG
    kprintf("ethinit(dev) called\n");
#endif    
    etptr = &eth[devptr->dvminor];

    /* read the ID prom */
    getidprom(&idprom, sizeof (struct idprom));
    if (idprom.id_format != 1) {
	panic("Can't read ID prom\n");
    }

    le = (struct le_device *) devptr->dvcsr;

    /* must start at word boundary, getdmem() takes care of that */
    ib = (struct le_init_block *) (getdmem(sizeof(struct le_init_block)));
				   
    /* allocate the ring buffers */
    /* we fudge, because these MUST be on a 4 word boundry */
    pmdrecv = (struct le_md (*)[])
      ((getdmem(7+sizeof(struct le_md)*EP_NUMRCV)+7) & ~(unsigned long) 7);

    /* we just use ONE transmit buffer */
    pmdxmit = (struct le_md *)
      ((getdmem(7+sizeof(struct le_md))+7) & ~(unsigned long) 7);

    /* zero-out the init block and ring pointers */
    bzero(ib, sizeof(struct le_init_block));
    
    for (i = 0; i < EP_NUMRCV; ++i)
      bzero(&(*pmdrecv)[i], sizeof(struct le_md));
    
    bzero(pmdxmit, sizeof(struct le_md));

    /* set the physical address for this machine */
    ib->ib_padr[0] = idprom.id_ether[1]; /* must byte swap */
    ib->ib_padr[1] = idprom.id_ether[0];
    ib->ib_padr[2] = idprom.id_ether[3];
    ib->ib_padr[3] = idprom.id_ether[2];
    ib->ib_padr[4] = idprom.id_ether[5];
    ib->ib_padr[5] = idprom.id_ether[4];

    /* put my physical address into the eth structure */
    blkcopy(etptr->etpaddr, idprom.id_ether, EP_ALEN);

#ifdef DEBUG
    kprintf("Physical address: %02x.%02x.%02x.%02x.%02x.%02x\n",
	    etptr->etpaddr[0], etptr->etpaddr[1], etptr->etpaddr[2],
	    etptr->etpaddr[3], etptr->etpaddr[4], etptr->etpaddr[5]);
    
#endif	    

    /* broadcast address */
    blkcopy(etptr->etbcast, EP_BRC, EP_ALEN);

    blkcopy(ib->ib_ladrf, "\0\0\0\0\0\0\0\0", 8);
    
    /* set up receive ring */
    ib->ib_rdrp.drp_len   = EP_NUMRCVL2;
    ib->ib_rdrp.drp_laddr = low16(LANCE_ADDR((char *)pmdrecv));
    ib->ib_rdrp.drp_haddr = hi8(LANCE_ADDR((char *)pmdrecv));
    
    /* set up xmit ring (we only use one buffer) */
    ib->ib_tdrp.drp_len   = EP_NUMXMITL2;	
    ib->ib_tdrp.drp_laddr = low16(LANCE_ADDR((char *)pmdxmit));
    ib->ib_tdrp.drp_haddr = hi8(LANCE_ADDR((char *)pmdxmit));

    /* set up the receive ring buffers */
    for (i=0; i<EP_NUMRCV; ++i) {
	/* set up the LANCE info */
	pmd = &(*pmdrecv)[i];
	
	/* we assume the network buffer pool has been initialized */
	pep = (struct ep *) getbuf(Net.netpool);
	pbufr = (char *)&pep->ep_eh;
	
	pmd->lmd_ladr = low16(LANCE_ADDR(pbufr));
	pmd->lmd_hadr = hi8(LANCE_ADDR(pbufr));
	pmd->lmd_bcnt = -(EP_EHLEN + EP_DLEN + EP_CRC);
	pmd->lmd_mcnt = 0;
	pmd->lmd_flags = LMD_OWN;	/* owned by LANCE */
	
	/* set up the XINU info */
	pbre = &(etptr->etbrr[i]);
	pbre->pmd = pmd;
	pbre->buf = (char *) pep;
    }
    
    /* set up the xmit ring buffers */
    /* set up the LANCE info */
    pmd = pmdxmit;
    pmd->lmd_bcnt = -(EP_EHLEN + EP_DLEN);
    pmd->lmd_mcnt = 0;
    pmd->lmd_flags = 0;			/* owned by host */
    
    /* set up the XINU info */
    pbre = &(etptr->etbrt[0]);
    pbre->pmd = pmd;
	
#ifdef DEBUG
    kprintf("Lance device at address 0x%x\n",(unsigned long) le);
    kprintf("Interrupt vector: 0x%x\n",devptr->dvivec);
    kprintf("Init block at 0x%lx\n",(unsigned long) ib);
    kprintf("  rladdr:0x%x   rhaddr:0x%x   len: %d\n",
	    ib->ib_rdrp.drp_laddr,
	    ib->ib_rdrp.drp_haddr,
	    ib->ib_rdrp.drp_len);
    kprintf("  tladdr:0x%x   thaddr:0x%x   len: %d\n",
	    ib->ib_tdrp.drp_laddr,
	    ib->ib_tdrp.drp_haddr,
	    ib->ib_tdrp.drp_len);
    kprintf("recv ring at 0x%lx\n",(unsigned long) pmdrecv);
    kprintf("xmit ring at 0x%lx\n",(unsigned long) pmdxmit);
    for (i=0; i<EP_NUMRCV; ++i) {
	kprintf("buf %d  pdm: 0x%x  buf: 0x%x-0x%x\n",
		i,
		etptr->etbrr[i].pmd,
		etptr->etbrr[i].buf,
		(long)etptr->etbrr[i].buf+EP_MAXLEN-1);
	pmd = &(*pmdrecv)[i];
	kprintf("  ladr:0x%04x  hadr:0x%02x   bcnt:0x%x  mcnt:0x%x flags:%d\n",
		pmd->lmd_ladr,
		pmd->lmd_hadr,
		pmd->lmd_bcnt,
		pmd->lmd_mcnt,
		pmd->lmd_flags);
    }
    kprintf(" tbuf  pdm: 0x%x  buf: 0x%x-0x%x\n",
	      etptr->etbrt[0].pmd,
	      etptr->etbrt[0].buf,
	      (long)etptr->etbrt[0].buf+EP_MAXLEN-1);
	pmd = pmdxmit;
	kprintf("  ladr:0x%04x  hadr:0x%02x   bcnt:0x%x  mcnt:0x%x flags:%d\n",
		pmd->lmd_ladr,
		pmd->lmd_hadr,
		pmd->lmd_bcnt,
		pmd->lmd_mcnt,
		pmd->lmd_flags);
    
    kprintf("\n");
#endif

    /* set up global information */
    devptr->dvioblk = (char *) etptr;
    etptr->etle   = le;
    etptr->etib   = ib;
    etptr->etdev  = devptr;
    etptr->etnextbuf = 0;
    etptr->etoutq = newq(ETOUTQSZ, QF_NOWAIT);
    etptr->etinq  = newq(ETINQSZ,  QF_NOWAIT);
    etptr->etrpid = BADPID;
    etptr->etrsem = screate(1);
    etptr->etintf = -1;
    etptr->etoutqlen = 0;	/* John */
    etptr->etrpending = 0;

#ifdef DEBUG
    kprintf("About to call ethstrt\n");
#endif
    
    ethstrt(etptr);
    return(OK);
}

