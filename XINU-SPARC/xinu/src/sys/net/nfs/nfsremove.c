/* nfsremove.c - nfsremove */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>


/*#define DEBUG*/
#define PRINTERRORS


/*
 * ====================================================================
 * nfsremove - try to remove an NFS file
 * ====================================================================
 */
nfsremove(path)
     char	*path;
{
	struct nfsblk *pnfs;
	int status;
	int retval;

#ifdef DEBUG
	kprintf("nfsremove(%s) called\n", path);
#endif

	/* initialize the nfsblk struct */
	pnfs = (struct nfsblk *) getmem(sizeof(struct nfsblk));
	pnfsopen(pnfs);
	

	/* look up the file */
	if ((retval = nfsfindfile(pnfs,path,NFS_MUST_EXIST,TRUE)) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsremove(%s) ==> SYSERR, no such file\n", path);
#endif
		pnfsclose(pnfs);
		freemem(pnfs,sizeof(struct nfsblk));
		return(SYSERR);
	}


	/* make the nfs call */
	wait(pnfs->nfs_pme->mnt_mutex);
	retval = nfscall(pnfs->nfs_pme->mnt_rpcdev, NFSPROC_REMOVE,
			 &pnfs->nfs_status,
			 pnfs->nfs_pme,
			 "%F %S",
			 pnfs->nfs_phandle, NFS_FHSIZE,	/* parent handle */
			 pnfs->nfs_fname, 0);		/* file name */

	signal(pnfs->nfs_pme->mnt_mutex);
	status = pnfs->nfs_status;
	pnfsclose(pnfs);
	freemem(pnfs,sizeof(struct nfsblk));

	if ((retval == SYSERR) || (status != NFS_OK)) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsremove: nfscall returns %d\n", retval);
#endif
		return(SYSERR);
	}

	return(OK);
}
