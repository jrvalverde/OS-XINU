/* nfsrename.c - nfsrename */

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
 * nfsrename - try to rename an NFS file
 *     "pathold" and "pathnew" MUST lie on the same NFS file system
 * ====================================================================
 */
nfsrename(pathold, pathnew)
     char	*pathold;
     char	*pathnew;
{
	struct nfsblk *pnfsold;
	struct nfsblk *pnfsnew;
	int status;
	int retval;

#ifdef DEBUG
	kprintf("nfsrename(%s,%s) called\n", pathold, pathnew);
#endif

	/* initialize the nfsblk structs */
	pnfsold = (struct nfsblk *) getmem(sizeof(struct nfsblk));
	pnfsopen(pnfsold);
	pnfsnew = (struct nfsblk *) getmem(sizeof(struct nfsblk));
	pnfsopen(pnfsnew);
	

	/* look up the paths and make sure they're valid */
	if (((retval = nfsfindfile(pnfsold,pathold,NFS_MUST_EXIST,TRUE)) == SYSERR) || 
	    ((retval = nfsfindfile(pnfsnew,pathnew,NFS_NEEDNT_EXIST,TRUE)) == SYSERR) || 
	    (pnfsold->nfs_pme != pnfsnew->nfs_pme)) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsrename(%s,%s) ==> SYSERR\n", pathold, pathnew);
#endif
		pnfsclose(pnfsold);
		pnfsclose(pnfsnew);
		freemem(pnfsold,sizeof(struct nfsblk));
		freemem(pnfsnew,sizeof(struct nfsblk));
		return(SYSERR);
	}


	/* make the nfs call */
	wait(pnfsold->nfs_pme->mnt_mutex);
	retval = nfscall(pnfsold->nfs_pme->mnt_rpcdev, NFSPROC_RENAME,
			 &pnfsold->nfs_status, pnfsold->nfs_pme,
			 "%F %S %F %S",
			 pnfsold->nfs_phandle, NFS_FHSIZE, /* parent handle */
			 pnfsold->nfs_fname, 0,		/* file name */
			 pnfsnew->nfs_phandle, NFS_FHSIZE, /* parent handle */
			 pnfsnew->nfs_fname, 0);	/* file name */

	signal(pnfsold->nfs_pme->mnt_mutex);
	status = pnfsold->nfs_status;
	pnfsclose(pnfsold);
	pnfsclose(pnfsnew);
	freemem(pnfsold,sizeof(struct nfsblk));
	freemem(pnfsnew,sizeof(struct nfsblk));

	if ((retval == SYSERR) || (status != NFS_OK)) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsrename: nfscall returns %d\n", retval);
#endif
		return(SYSERR);
	}

	return(OK);
}
