/* nfscall.c - nfscall */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>
#include <varargs.h>


/*#define DEBUG*/
#define PRINTERRORS

/*
 * ====================================================================
 * nfscall - call the given procedure with the arguments given
 * ====================================================================
 */
/*VARARGS5*/
nfscall(dev, proc, pstatus, pme, format, va_alist)
     int		dev;
     unsigned		proc;
     unsigned		*pstatus;
     struct mountentry	*pme;
     char		*format;
     va_dcl
{
    va_list args;

#ifdef DEBUG
    kprintf("nfscall(%d,%s,args) called\n", proc, format);
#endif

    /* optimistic, for now */
    *pstatus = NFS_OK;
	
    /* change to the requested procedure */
    if (control(dev, RPC_SETPROC, proc) != OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfscall: failed to set PROC to %d\n", proc);
#endif
	*pstatus = NFSERR_RPCERR;
	return(SYSERR);
    }
	
    /* set the arguments */
    va_start(args);
    if (control(dev, XSET_FORMAT, format,args) != OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfscall: failed to set arguments\n");
#endif
	*pstatus = NFSERR_RPCERR;
	return(SYSERR);
    }
    va_end(args);

	
    /* make the RPC call */
    if (control(dev, RPC_CALL) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfscall: RPC_CALL returns SYSERR\n");
#endif
	*pstatus = NFSERR_RPCERR;
	return(SYSERR);
    }

    /* get the status */
    if (control(dev, XGET_UINT, pstatus) != OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfscall: error reading status\n");
#endif
	*pstatus = NFSERR_RPCERR;
	return(SYSERR);
    }

    /* see how long it took for RTO estimation */
    control(dev,RPC_GETRTO, &pme->mnt_timeout);

#if defined(DEBUG)
    if (*pstatus != NFS_OK) {
	kprintf("nfscall: nfs_status: %s\n", nfsStatString(*pstatus));
    }
#endif
    return(OK);
}
