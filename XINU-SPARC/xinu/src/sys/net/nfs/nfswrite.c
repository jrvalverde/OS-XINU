/* nfswrite.c - nfswrite */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>

/*#define DEBUG*/
/*#define DEBUG_VERBOSE*/
#define PRINTERRORS

/* local routines */
static nfsDoWrite();

/*-------------------------------------------------------------------------
 * nfswrite - 
 *-------------------------------------------------------------------------
 */
nfswrite(devptr, buff, len)
     struct devsw *devptr;
     char *buff;
     unsigned len;
{
    struct nfsblk *pnfs;
    int retval;
    unsigned totalLen;
    unsigned toWrite;

#ifdef DEBUG_VERBOSE
    kprintf("nfswrite(devptr,0x%08x,%d) called\n", buff, len);
#endif

    pnfs = (struct nfsblk *) devptr->dvioblk;

    wait(pnfs->nfs_mutex);
	
    pnfs->nfs_status = NFS_OK;
    totalLen = 0;

    while (len) {
	toWrite = ((len > NFS_MAXDATA) ? NFS_MAXDATA : len);
	retval = nfsDoWrite(pnfs, buff + totalLen, toWrite);
	if (retval == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("nfsWrite: error %s\n",
		    nfsStatString(pnfs->nfs_status));
#endif
	    /* MOREWORK do we return what we got so far? */
	    /* hmm.  we probably didn't get anything, though */
	    signal(pnfs->nfs_mutex);
	    return(SYSERR);
	}

	totalLen += retval;
	len -= retval;
	pnfs->nfs_fpos += retval;
	if (retval < toWrite)
	    break;
    }
    signal(pnfs->nfs_mutex);
    return(totalLen);
}



/*-------------------------------------------------------------------------
 * nfsDoWrite - 
 *-------------------------------------------------------------------------
 */
LOCAL nfsDoWrite(pnfs, buff, len)
	struct nfsblk *pnfs;
	char *buff;
	unsigned len;
{
    int retval;
    int dev;

#ifdef DEBUG_VERBOSE
    kprintf("nfsDoWrite(devptr,0x%08x,%d) called\n", buff, len);
#endif

    dev = pnfs->nfs_rpcdev;
    /* make the nfs call */
    retval = nfscall(dev, NFSPROC_WRITE, &pnfs->nfs_status,
		     pnfs->nfs_pme,
		     "%F%U%U%U%U%F",
		     pnfs->nfs_fhandle, NFS_FHSIZE, /* handle */
		     0,			/* totallen unused */
		     pnfs->nfs_fpos,	/* file pos */
		     0,			/* totallen unused */
		     len,		/* write len */
		     buff, len);	/* the data */

    if (retval != OK || pnfs->nfs_status != NFS_OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsDoWrite: error retval:%d status:%s\n",
		retval, nfsStatString(pnfs->nfs_status));
#endif
	return(SYSERR);
    }
    /* get the latest file attributes */
    retval = rpcscanf(dev, "{17%U}", &pnfs->nfs_attributes);
    if (retval != OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsDoWrite: couldn't read results\n");
#endif
	return(SYSERR);
    }

    return(len);
}
