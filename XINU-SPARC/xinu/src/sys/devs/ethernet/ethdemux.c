/* ethdemux.c - ethdemux */

#include <conf.h>
#include <kernel.h>
#include <lereg.h>
#include <network.h>
#include <vmem.h>
#include <proc.h>


/*#define DEBUG*/
#define PRINTERRORS

/*------------------------------------------------------------------------
 *  ethdemux - select a port to send an incoming packet
 *------------------------------------------------------------------------
 */
int ethdemux(pet, pep, readlen)
     struct	etblk	*pet;
     struct	ep	*pep;
     int	readlen;
{
    int ifnum;
    
#ifdef DEBUG
	kprintf("ethdemux called: packet (0x%08x)\n", pep);
#endif
    
    ifnum = pet->etintf;
    /* check for a process waiting to read */
    if (pet->etrpid != BADPID) {
#ifdef DEBUG
	kprintf("ethdemux: passed packet to proc %d\n", pet->etrpid);
#endif
	/* pass the pep to the reading process */
	if (enq(pet->etinq, pep, 0) < 0) {
#if defined(PRINTERRORS) || defined(DEBUG)
	    kprintf("ethdemux() couldn't enq()\n");
#endif
	    freebuf(pep);
	    return(SYSERR);
	}
	
	/* wake up the process */
	ready(pet->etrpid, RESCHNO);
	pet->etrpid = BADPID;
    } else {
#ifdef DEBUG
	kprintf("ethdemux: passed packet through ni_in\n");
#endif
	/* deliver the pep to the interface */
	ifnum = pet->etintf;
	if (ifnum < 0 || nif[ifnum].ni_state != NIS_UP) {
	    nif[ifnum].ni_idiscard++;
	    freebuf(pep);
	    return OK;
	}
	ni_in(&nif[ifnum], pep, readlen);
    }
#ifdef DEBUG
    kprintf("ethdemux: exit\n");
#endif
    
    return OK;
}
