/* RPC Portmapper
 *
 * Author:	Brian K. Grant
 *		Dept. of Computer Sciences
 *		Purdue University
 *
 * Written:	11-10-91
 */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <xdr.h>
#include <network.h>
#include <rpc.h>
#include <portmapper.h>
#include <rpcprogs.h>
#include <pmapd.h>

/*#define DEBUG*/
/*#define DEBUG_VERBOSE*/

#ifdef DEBUG_VERBOSE
/* For debugging messages */
static char *pmap_proc_names[] = PMAP_PROC_NAMES;
#endif DEBUG_VERBOSE



/*----------------------------------------------------------------------------
 * pmapinit -- open the RPC server device
 *----------------------------------------------------------------------------
 */
int pmapinit()
{
    int dev;
    char strprog[24];

    /* Open the port */
    sprintf(strprog, "%d:%d", PMAP_PROG, PMAP_VERS);
    if (SYSERR == (dev = open(RPC, strprog, RPCT_LSERVER))) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("%s: Could not open RPC server device\n", PMAPDNAM);
#endif
	return (SYSERR);
    }

    return (dev);
}


/*----------------------------------------------------------------------------
 * PMAPP -- process and respond to portmapper calls
 *----------------------------------------------------------------------------
 */
PROCESS PMAPP(dev)
    int dev;
{
    unsigned proc, prog, vers, prot, port;

    /* We're not checking the auth. fields */
    if (pmapskipauth(dev) == SYSERR) {
#ifdef DEBUG_VERBOSE
	kprintf("%s: Error from pmapskipauth\n", PMAPPNAM);
#endif
	close(dev);
	return (SYSERR);
    }
    
    control(dev, RPC_GETPROC, &proc);
#ifdef DEBUG_VERBOSE
    if ((proc >= PMAP_PROC_NULL) && (proc <= PMAP_PROC_CALLIT)) {
	kprintf("%s: Procedure %s called\n", PMAPPNAM,
		pmap_proc_names[proc]);
    }
#endif    
    switch (proc) {
      case PMAP_PROC_NULL:
	/* we don't have to do anything here */
	break;
	
      case PMAP_PROC_SET:
	if (pmapgetmapping(dev, &prog, &vers, &prot, &port) == SYSERR) {
#ifdef DEBUG_VERBOSE
	    kprintf("%s: Error from pmapgetmapping\n", PMAPPNAM);
#endif
	    close(dev);
	    return (SYSERR);
	}
	pmapset(dev, prog, vers, prot, port);
	break;
	
      case PMAP_PROC_UNSET:
	if (pmapgetmapping(dev, &prog, &vers, &prot, &port) == SYSERR) {
#ifdef DEBUG_VERBOSE
	    kprintf("%s: Error from pmapgetmapping\n", PMAPPNAM);
#endif
	    close(dev);
	    return (SYSERR);
	}
	pmapunset(dev, prog, vers, prot, port);
	break;
	
      case PMAP_PROC_GETPORT:
	if (pmapgetmapping(dev, &prog, &vers, &prot, &port) == SYSERR) {
#ifdef DEBUG_VERBOSE
	    kprintf("%s: Error from pmapgetmapping\n", PMAPPNAM);
#endif
	    close(dev);
	    return (SYSERR);
	}
	pmapgetport(dev, prog, vers, prot);
	break;
	
      case PMAP_PROC_DUMP:
	pmapdump(dev);
	break;
	
      case PMAP_PROC_CALLIT:
	pmapgetcallargs(dev, &prog, &vers, &proc);
	if (pmapcallit(dev, prog, vers, proc) == SYSERR) {
#ifdef DEBUG_VERBOSE
	    kprintf("%s: Error in procedure %s\n", PMAPPNAM,
		    pmap_proc_names[PMAP_PROC_CALLIT]);
#endif    
	    close(dev);
	    return (SYSERR);
	}
	break;
	
      default:
#ifdef DEBUG_VERBOSE
	kprintf("%s: Unknown procedure called: %u\n", PMAPPNAM, proc);
#endif
	control(dev, RPC_REPLY, RPC_ERROR_PROCUNAVAIL);
	close(dev);
	return (SYSERR);
    }

    control(dev, RPC_REPLY, RPC_OK);
    close(dev);
    return (OK);
}


/*----------------------------------------------------------------------------
 * PMAPD -- wait for RPC (RFC 1057) messages and respond (UDP only)
 *----------------------------------------------------------------------------
 */
PROCESS PMAPD()
{
    int dev;
    int adev;

    /* Open the port */
    if (SYSERR == (dev = pmapinit())) {
	   kill(getpid());
    }

    /* Wait for requests -- loop forever */
    while (TRUE) {
	/* listen on the port */
#ifdef DEBUG_VERBOSE
	kprintf("%s: Listening on the port\n", PMAPDNAM);
#endif
	if (SYSERR == control(dev, RPC_LISTEN, &adev)) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("%s: Error in RPC_LISTEN!!!\n", PMAPDNAM);
#endif
	} else if (SYSERR == resume(kcreate(PMAPP,
					    PMAPSTK,
					    PMAPPRI,
					    PMAPPNAM,
					    1, adev))) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("%s: Could not create process %s\n",
			PMAPDNAM, PMAPPNAM);
#endif
		/* Just do it linearly */
		PMAPP(adev);
        }
    }
}
