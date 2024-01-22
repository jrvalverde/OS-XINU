/* vbcopy.c - vbcopy */

/*#define DEBUG*/

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <q.h>
#include <proc.h>
#include <vmem.h>

/*
 *------------------------------------------------------------------------
 * vbcopy - copy contents from one process's address space to another.
 *	    This is a general purpose copy routine.  If you are copying
 *	    an entire page, then call physpgcopy().  It is assumed
 *	    interrupts are disabled.
 *	    MACHINE INDEPENDENT
 *------------------------------------------------------------------------
 */
vbcopy(vaddrfrom, fromid, vaddrto, toid, nbytes, setmbit)
char *vaddrfrom;		/* virt address copying from	*/
int fromid;			/* process id copying from	*/
char *vaddrto;			/* virt address copying to	*/
int toid;			/* process id copying to	*/
register int nbytes;		/* number of bytes to be copyied*/
Bool setmbit;			/* set the modified bit?	*/
{
    PStype  ps;				/* save process register 	*/
    char	*virtaddr;		/* virt addr not in curid space	*/
    int	vid;				/* pid associated with virtaddr	*/
    char	*pysaddr;		/* physical address		*/
    int	framenum;			/* frame nubmer			*/
    register int byteoffset;		/* byte within page		*/
    char	*sysaddr;		/* system address		*/
    char	*copyaddr;		/* begining of copy page 	*/
    struct  ppte  ppte;			/* pte structure		*/
    struct  ppte  *ppteptr;		/* ppte structure pointer       */
    int	copysize;			/* size of area to copy		*/
    int	i;				/* counter variable		*/
	

#ifdef DEBUG
    kprintf("in vbcopy\n");
    kprintf("vaddrfrom=%08x, fromid=%d, vaddrto=%08x, toid=%d, nbytes=%d, setmbit=%d\n", vaddrfrom, fromid, vaddrto, toid, nbytes, setmbit);
#endif
    if (nbytes==0)
	return(OK);
	
    if (isbadpid(fromid) && isbadasid(fromid) || isbadpid(toid)
	&& isbadasid(toid) )
	return(SYSERR);	
	
    if ( toid == currpid ) {
	vid = fromid;
	virtaddr = vaddrfrom;
    }
    else if ( fromid == currpid ) {
	vid = toid;
	virtaddr = vaddrto;
    }
    else 
	return(SYSERR);			/* one of them must be current pid */
	
	
    if ((unsigned long)virtaddr >= (unsigned long)vmaddrsp.xinu) {
	if ( fromid == currpid ) {
	    for ( ; nbytes > 0 ; --nbytes) {
		*virtaddr++ = *vaddrfrom++;
	    }
	}
	else {
	    for ( ; nbytes > 0 ; --nbytes) {
		*vaddrto++ = *virtaddr++;
	    }
	}
	restore(ps);
	return(OK);
    }
	
    ppteptr = &ppte;
    for ( ; nbytes > 0; ) {
		
#ifdef DEBUG
	kprintf("vid=%d, virtaddr=%08x\n", vid, virtaddr);
#endif
	getppte(vid, virtaddr, ppteptr);
	
#ifdef DEBUG
	kprintf("getppte( %d, %08x, %08x)\n", vid, virtaddr, ppteptr);
	kprintf("inmmu=%d, pmeg=%d, ptenum=%d\n",
		ppteptr->inmmu, ppteptr->loc.mmuindex.pmeg,
		ppteptr->loc.mmuindex.ptenum);
#endif
	if (!getinmem(ppteptr))
	    return(SYSERR);
	
	framenum = getpfn(ppteptr);
#ifdef DEBUG
	kprintf("framenum = %d\n", framenum);
#endif
	if (setmbit)
	    setmod(ppteptr, TRUE);
	
	byteoffset = tooffset(virtaddr);

	/* pte at vmaddrsp.copyaddr is reserved for copying purpose */
	copyaddr = vmaddrsp.copyaddr;
#ifdef DEBUG
	kprintf("getppte( %d, %08x, %08x)\n", currpid, copyaddr, ppteptr);
#endif
	getppte(currpid, copyaddr, ppteptr);
		
#ifdef DEBUG
	kprintf("inmmu=%d, pmeg=%d, ptenum=%d\n",
		ppteptr->inmmu, ppteptr->loc.mmuindex.pmeg,
		ppteptr->loc.mmuindex.ptenum);
#endif
	/* map to frame in the other address space */
	setvalue(ppteptr, (int)0);
	setpfn(ppteptr, framenum);
	sethvalid(ppteptr, TRUE);
	settvalid(ppteptr, TRUE);
	setprot(ppteptr, URW_KRW);
		
	sysaddr = copyaddr + byteoffset;
		
	copysize = ((byteoffset+nbytes) > PGSIZ) ? (PGSIZ-byteoffset): nbytes;
	
#ifdef USE_THE_VAC		/* only flush if VAC is being used */
	vac_flushContext();	/* flush the VAC */
#endif
	if (fromid == currpid) {
	    for (i=0; i<copysize; i++)
		*sysaddr++ = *vaddrfrom++;
	}
	else {
	    for (i=0; i<copysize; i++)
		*vaddrto++ = *sysaddr++;
	}

	nbytes -= copysize;
	virtaddr += copysize;
	/* across page boundary, next page begins*/
    }
	
    /* return pointer to address beyond last byte copied */
    return((unsigned) sysaddr);
}





