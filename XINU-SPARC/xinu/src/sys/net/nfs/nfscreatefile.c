/* nfscreatefile.c - nfscreatefile */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>


/*#define DEBUG*/
#define PRINTERRORS


static struct sattr createsattr = {
	NFS_CREATE_MODE_FILE,	/* file mode		*/
	NFS_CREATE_UID,		/* uid			*/
	NFS_CREATE_GID,		/* gid			*/
	0,			/* size of file		*/
	-1,-1,			/* atime		*/
	-1,-1			/* mtime		*/
};


/*
 * ====================================================================
 * nfscreatefile - create the file whose name and parent handle are
 *                 contained in the nfsblk structure passed
 * ====================================================================
 */
nfscreatefile(pnfs)
	struct nfsblk *pnfs;
{
	int retval;
	int dev;

#ifdef DEBUG
	kprintf("nfscreatefile(%s) called\n", pnfs->nfs_fpath);
#endif

	/* we use the RPC device from the mount table entry */
	dev = pnfs->nfs_pme->mnt_rpcdev;
	wait(pnfs->nfs_pme->mnt_mutex);

	/* just make the call */
	retval = nfscall(dev, NFSPROC_CREATE, &pnfs->nfs_status, pnfs->nfs_pme,
			 "%F %S {8%U}",
			 pnfs->nfs_phandle, NFS_FHSIZE,
			 pnfs->nfs_fname, 0,
			 &createsattr.sa_mode);

	if (retval == SYSERR || pnfs->nfs_status != NFS_OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfscreatefile: nfsCall returns %d\n", retval);
#endif
		signal(pnfs->nfs_pme->mnt_mutex);
		return(SYSERR);
	}
	
	/* get the new file's handle and attributes */
	retval = rpcscanf(dev, "%F {17%U}",
			  pnfs->nfs_fhandle, NFS_FHSIZE,
			  &pnfs->nfs_attributes);

	signal(pnfs->nfs_pme->mnt_mutex);

	if (retval == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfscreatefile: RPC error reading results\n");
#endif
		pnfs->nfs_status = NFSERR_RPCERR;
		return(SYSERR);
	}

	return(OK);
}
