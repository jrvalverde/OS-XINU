/* nfsymlink - nfssymlink */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>


/*#define DEBUG*/
#define PRINTERRORS


/* routines used */
char *index();
char *getmem();


/*
 * ====================================================================
 * nfssymlink - return the value of a symbolic link
 * ====================================================================
 */
nfssymlink(pnfs, handle, oldpath, path)
     struct nfsblk *pnfs;
     fhandle handle;
     char *path;
     char *oldpath;
{
	int retval;
	int dev;
	char *newpath;

#ifdef DEBUG
	kprintf("nfssymlink() called\n");
#endif

	/* we use the RPC device from the mount table entry */
	dev = pnfs->nfs_pme->mnt_rpcdev;
	wait(pnfs->nfs_pme->mnt_mutex);

	/* make an NFS call to read the link */
	retval = nfscall(dev, NFSPROC_READLINK, &pnfs->nfs_status,
			 pnfs->nfs_pme,
			 "%F",
			 handle, NFS_FHSIZE);

	if (retval == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfssymlink: got error from nfscall: %d\n", retval);
#endif
		pnfs->nfs_status = NFSERR_RPCERR;
		signal(pnfs->nfs_pme->mnt_mutex);
		return(SYSERR);
	}

	if (pnfs->nfs_status != NFS_OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfssymlink: bad status from nfscall\n");
#endif
		signal(pnfs->nfs_pme->mnt_mutex);
		return(SYSERR);
	}

	/* create a buffer and grab the string */
	newpath = getmem(NFS_PATHLEN);
	control(dev, XGET_STRING, newpath, NFS_PATHLEN);
#ifdef DEBUG
	kprintf("nfssymlink: points to %s\n", newpath);
#endif

	signal(pnfs->nfs_pme->mnt_mutex);

	/* append a slash and whatever is left from the old path */
	if (*oldpath) {
		strcat(newpath,"/");
		strcat(newpath,oldpath);
	}

#ifdef DEBUG
	kprintf("nfssymlink: final path is %s\n", newpath);
#endif

	/* copy the new path to the old */
	strcpy(path,newpath);
	freemem(newpath,NFS_PATHLEN);

	return(OK);
}
