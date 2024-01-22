/* nfsread.c - nfsread */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>


/*#define DEBUG*/
/*#define DEBUG_VERBOSE*/
/*#define PRINTERRORS*/

/* local routines */
static nfsDoRead();

/*-------------------------------------------------------------------------
 * nfsread - 
 *-------------------------------------------------------------------------
 */
nfsread(devptr, buff, len)
     struct	devsw	*devptr;
     char	*buff;
     unsigned	len;
{
    struct nfsblk *pnfs;
    int retval;
    unsigned totalCount;
    unsigned toRead;

#ifdef DEBUG
    kprintf("nfsread(%d,0x%x,%d) called\n", devptr->dvnum, buff, len);
#endif

    pnfs = (struct nfsblk *) devptr->dvioblk;

    wait(pnfs->nfs_mutex);

    pnfs->nfs_status = NFS_OK;
    totalCount = 0;

    while (len) {
	toRead = ((len > NFS_MAXDATA) ? NFS_MAXDATA : len);
	retval = nfsDoRead(pnfs, buff + totalCount, toRead);

	if (retval == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("nfsRead: error retval:%d status:%s\n",
		    retval, nfsStatString(pnfs->nfs_status));
#endif
	    /* MOREWORK do we return what we got so far? */
	    /* hmm.  we probably didn't get anything, though */
	    signal(pnfs->nfs_mutex);
	    return(SYSERR);
	}

	totalCount += retval;
	len -= retval;
	pnfs->nfs_fpos += retval;
	if (retval < toRead)
	    break;
    }

#ifdef DEBUG
    kprintf("nfsread returns %d\n", totalCount);
#endif
    signal(pnfs->nfs_mutex);

    return(totalCount);
}




/*-------------------------------------------------------------------------
 * nfsDoRead - 
 *-------------------------------------------------------------------------
 */
static nfsDoRead(pnfs, buff, len)
	struct nfsblk *pnfs;
	char *buff;
	unsigned len;
{
    int retval;
    int dev;
    unsigned inlength;

#ifdef DEBUG_VERBOSE
    kprintf("nfsDoRead(buff:0x%08x, len:%d) called\n", buff, len);
#endif

    dev = pnfs->nfs_rpcdev;

    /* make the nfs call */
    retval = nfscall(dev, NFSPROC_READ, &pnfs->nfs_status,
		     pnfs->nfs_pme,
		     "%F %U %U %U",
		     pnfs->nfs_fhandle, NFS_FHSIZE, /* file handle	*/
		     pnfs->nfs_fpos,	/* file position */
		     len,		/* read len	*/
		     0);		/* totalcount (unused) */

    if ((retval != OK) || (pnfs->nfs_status != NFS_OK)) {
	return(SYSERR);
    }

    /* pull out the reply and data */
    inlength = len;
    retval = rpcscanf(dev, "{17%U} %V",
		      &pnfs->nfs_attributes, /* attributes	*/
		      buff, &inlength);	/* the data	*/
    if (retval != OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsDoRead: couldn't read results\n");
#endif
	pnfs->nfs_status = NFSERR_RPCERR;
	return(SYSERR);
    }

#ifdef DEBUG_VERBOSE
    kprintf("nfsDoRead: returning length %d\n", inlength);
#endif

    return(inlength);
}
