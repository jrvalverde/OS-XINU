/* physpgcopy.c - physpgcopy */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <q.h>
#include <proc.h>
#include <vmem.h>

/*
 *------------------------------------------------------------------------
 * physpgcopy - copy a single page from one process's address space to another.
 *	Use vbcopy() for general copying between address spaces.  This
 *	routine is only to be used to copy a portion (or entire) page between
 *	processes.  It is also assumed that vaddrfrom and vaddrto
 *	are on page boundaries.  It is assumed that nbytes is in BYTES and
 *	is a multiple of the size of an int. It is assumed interrupts
 *	are disabled.
 *	MACHINE INDEPENDENT
 *------------------------------------------------------------------------
 */
physpgcopy(vaddrfrom, fromid, vaddrto, toid, nbytes, copypage, setmbit)
unsigned int  *vaddrfrom;	/* virt address copying from	*/
int fromid;			/* process id copying from	*/
unsigned int  *vaddrto;		/* virt address copying to	*/
int toid;			/* process id copying to	*/
char copypage;			/* which copy page is being used */
int nbytes;			/* how much of page to copy in BYTES */
Bool setmbit;			/* set the modified bit?	*/
{
    PStype  ps;
    int	framenum;			/* frame nubmer			*/
    int	*sysaddr;			/* system address		*/
    struct  ppte  ppte;
    struct  ppte  *ppteptr;		/* ppte structure pointer       */


    if ((nbytes > PGSIZ) || (((unsigned)vaddrfrom % sizeof(int)) != 0) ||
	(((unsigned)vaddrto % sizeof(int)) != 0) || ((nbytes%sizeof(int))!=0)){
	kprintf("physpgcopy: returning SYSERR\n");
	kprintf("     	     vaddrfrom = %08x, vaddrto = %08x, nbytes = %d",
		vaddrfrom, vaddrto, nbytes);
	return(SYSERR);
    }
    
    ppteptr = &ppte;
    if (fromid == currpid) {
	getppte( toid, vaddrto, ppteptr );
    }
    else if (toid == currpid ) {
	getppte( fromid, vaddrfrom, ppteptr );
    }
    else  {
	kprintf("physpgcopy: returning SYSERR: \n");
	kprintf("            toid = %d, fromid = %d, currpid = %d\n",
		toid, fromid, currpid);
	return(SYSERR);
    }
		
	
    if (!getinmem(ppteptr)) {		/* make sure page is there	*/
	kprintf("physpgcopy returning SYSERR: pg not in mem!\n");
	return(SYSERR);
    }
		
    framenum = getpfn(ppteptr);

    if (setmbit)
	setmod(ppteptr, TRUE);
	
    if (copypage == PGSCOPY) {
	sysaddr = (int *)vmaddrsp.pgscpaddr;
    }
    else if (copypage == PGRCOPY) {
	sysaddr = (int *)vmaddrsp.pgrcpaddr;
    }
    else {
	sysaddr = (int *)vmaddrsp.copyaddr;
    }

    getppte(currpid, sysaddr, ppteptr);
    
    setvalue(ppteptr, (int)0);
    setpfn(ppteptr, framenum);
    sethvalid(ppteptr, TRUE);
    settvalid(ppteptr, TRUE);
    setprot(ppteptr, URW_KRW);
    
#ifdef USE_THE_VAC		/* only flush if VAC is being used */
    vac_flushContext();	/* flush the VAC */
#endif
	
    if (fromid == currpid) 
	fpgcpy(vaddrfrom, sysaddr, nbytes/sizeof(int));
    else 
	fpgcpy(sysaddr, vaddrto, nbytes/sizeof(int));

    /* return pointer to address beyond last byte copied */
    return((unsigned)(sysaddr+(nbytes/sizeof(int))));	
}



/*---------------------------------------------------------------------------
 * fpgcpy - fast page copy using ints (use entire BUS width)
 *		'fromaddr' and 'toaddr' are assumed to be on int boundaries.
 *		'numints' is the number of ints to copy
 *---------------------------------------------------------------------------
 */
fpgcpy(fromaddr, toaddr, numints)
register int *fromaddr;
register int *toaddr;
register int numints;
{
    while (numints--)
	*toaddr++ = *fromaddr++;
}
