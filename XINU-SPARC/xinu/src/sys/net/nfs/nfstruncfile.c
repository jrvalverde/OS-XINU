/* nfstruncfile.c - nfstruncfile */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>


/*#define DEBUG*/
#define PRINTERRORS


static struct sattr truncsattr = {
	-1,		/* file mode		*/
	-1,		/* uid			*/
	-1,		/* gid			*/
	0,		/* size of file		*/
	-1,-1,		/* atime		*/
	-1,-1		/* mtime		*/
};


/*
 * ====================================================================
 * nfstruncfile - truncate an opened file to 0 length
 * ====================================================================
 */
nfstruncfile(pnfs)
	struct nfsblk *pnfs;
{
	int retval;
	int dev;

#ifdef DEBUG
	kprintf("nfstruncfile(%s) called\n", pnfs->nfs_fpath);
#endif

	/* we use the RPC device from NFS device */
	dev = pnfs->nfs_rpcdev;

	/* just make the call */
	retval = nfscall(dev, NFSPROC_SETATTR, &pnfs->nfs_status,
			 pnfs->nfs_pme,
			 "%F {8%U}",
			 pnfs->nfs_fhandle, NFS_FHSIZE,
			 &truncsattr);

	if (retval == SYSERR || pnfs->nfs_status != NFS_OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfstruncfile: nfsCall returns %d\n", retval);
#endif
		return(SYSERR);
	}
	
	/* get the file's new attributes */
	if (rpcscanf(dev, "{17%U}", &pnfs->nfs_attributes) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfstruncfile: RPC error reading results\n");
#endif
		pnfs->nfs_status = NFSERR_RPCERR;
		return(SYSERR);
	}

	/* set the file position to the beginning */
	pnfs->nfs_fpos = 0;

	return(OK);
}
