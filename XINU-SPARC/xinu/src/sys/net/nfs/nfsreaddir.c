/* nfsreaddir.c - nfsreaddir */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>


/*#define DEBUG*/
#define PRINTERRORS

/*-------------------------------------------------------------------------
 * nfsreaddir - return a number of structures containing the filenames
 *              in an opened directory.  This routine does no caching,
 *              each request goes back to the NFS server, so ask for
 *              as many at a time as is "reasonable".
 *-------------------------------------------------------------------------
 */
nfsreaddir(pnfs, preq)
     struct nfsblk *pnfs;
     struct nfs_readdir_req *preq;
{
    struct nfs_readdir_entry ent_junk;
    struct nfs_readdir_entry *pent;
    int count;
    int nbytes;
    int retval;
    int dev;
    int len;
    Bool more;

#ifdef DEBUG
    kprintf("nfsreaddir(%s,cookie:0x%x,nentries:%d) called\n",
	    pnfs->nfs_fpath, preq->cookie, preq->nentries);
#endif

    /* we use the RPC device from the nfs struct */
    dev = pnfs->nfs_rpcdev;

    /* just make the call */
    nbytes = preq->nentries * sizeof(struct nfs_readdir_entry);
#ifdef DEBUG
    kprintf("nbytes: %d\n", nbytes);
#endif
    retval = nfscall(dev, NFSPROC_READDIR, &pnfs->nfs_status,
		     pnfs->nfs_pme,
		     "%F %U %U",
		     pnfs->nfs_fhandle, NFS_FHSIZE,
		     preq->cookie,
		     nbytes);

    if (retval == SYSERR || pnfs->nfs_status != NFS_OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
	kprintf("nfsreaddir: nfsCall returns %d\n", retval);
#endif
	return(SYSERR);
    }

    /* get each of the entries */
    for (count = 0; ; ++count) {
	/* check for another response */
	control(dev, XGET_BOOL, &more);
	if (!more)
	    break;

	if (count < preq->nentries)
	    pent = &preq->pent[count];
	else
	    pent = &ent_junk;

	len = NFS_FILELEN;
	retval = rpcscanf(dev, "%U%S%U",
			  &pent->fileid,
			  pent->filename, &len,
			  &pent->cookie);
#ifdef DEBUG
	kprintf("  %3d: fileid:0x%08x  cookie:0x%08x '%s'\n",
		count, pent->fileid, pent->cookie, pent->filename);
#endif
    }

    if (count <= preq->nentries) {
	control(dev, XGET_BOOL, &preq->eof);
	preq->nentries = count;
    } else {
	preq->eof = FALSE;
    }
    preq->cookie = preq->pent[preq->nentries-1].cookie;


#ifdef DEBUG
    kprintf("nfsreaddir: eof set to %d\n", preq->eof);
#endif

    return(OK);
}
