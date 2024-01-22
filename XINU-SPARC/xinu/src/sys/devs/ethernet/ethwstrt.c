/* ethwstrt.c - ethwstrt */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <lereg.h>
#include <vmem.h>

/*#define DEBUG*/

/*------------------------------------------------------------------------
 * ethwstrt - start an ethernet write operation on the LANCE
 * Interrupts are disabled before this routine is called
 *------------------------------------------------------------------------
 */
int ethwstrt(pet)
     struct	etblk	*pet;
{
    struct	le_device	*le;
    struct	le_md		*pmdxmit;
    struct	ep		*pep;
    int			len;
    Bool			bcast;
    
#ifdef DEBUG
    kprintf("ethwstrt called\n");
#endif	
    
    pep = (struct ep *) deq(pet->etoutq);
    if (pep == 0) {
	kprintf("!!ethwstrt(): no packet deq'd (qlen=%d)\n", pet->etoutqlen);
	pet->etoutqlen = 0;
	return OK;
    }
    
#ifdef DEBUG
    kprintf("ethwstrt() deq'd packet %x\n", pep);
#endif
    
    bcast = blkequ(pep->ep_dst, pet->etbcast, EP_ALEN);
    len = pep->ep_len;
    le = pet->etle;
    pmdxmit = eth->etbrt[0].pmd;
    pet->etwbuf = (char *) pep;
    pet->etwtry = EP_RETRY;
    
    /* give the new buffer to the LANCE chip */
    pmdxmit->lmd_ladr = low16(LANCE_ADDR((char *)&pep->ep_eh));
    pmdxmit->lmd_hadr = hi8(LANCE_ADDR((char *)&pep->ep_eh));

    /* set the buffer ring bits for the packet */
    pmdxmit->lmd_bcnt = -pep->ep_len;
    pmdxmit->lmd_mcnt = 0;
    pmdxmit->lmd_flags = LMD_OWN | LMD_STP | LMD_ENP;
    le->le_csr = LE_TDMD | LE_INEA;

    if (bcast)
	nif[pet->etintf].ni_onucast++;
    else
	nif[pet->etintf].ni_oucast++;
    nif[pet->etintf].ni_ooctets += len;
    
#ifdef DEBUG
    kprintf("ethwstrt done!!\n");
#endif
}

