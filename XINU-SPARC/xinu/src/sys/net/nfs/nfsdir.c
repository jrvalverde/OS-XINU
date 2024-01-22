/* nfsdir.c - nfsmkdir, nfsrmdir */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>
#include <name.h>


/*#define DEBUG*/
#define PRINTERRORS


static struct sattr mkdirsattr = {
	NFS_CREATE_MODE_DIR,	/* file mode		*/
	NFS_CREATE_UID,		/* uid			*/
	NFS_CREATE_GID,		/* gid			*/
	0,			/* size of file		*/
	-1,-1,			/* atime		*/
	-1,-1			/* mtime		*/
};




/*
 * ====================================================================
 * nfsmkdir - try to create a directory
 * ====================================================================
 */
nfsmkdir(path)
     char *path;
{
	struct nfsblk *pnfs;
	int status;
	char xname[NAMLEN];
	int retval;
	int dev;

#ifdef DEBUG
	kprintf("nfsmkdir(%s) called\n", path);
#endif
	
	/* run the name through the namespace */
	if (nammap(path,xname) != NFS) {
		/* couldn't be an NFS file */
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsrmdir(%s) ==> SYSERR, not an NFS file\n", path);
#endif
		return(SYSERR);
	}

	/* initialize the nfsblk struct */
	pnfs = (struct nfsblk *) getmem(sizeof(struct nfsblk));
	pnfsopen(pnfs);

	/* look up the file */
	retval = nfsfindfile(pnfs,xname,NFS_NEEDNT_EXIST,TRUE);
	if (retval == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsmkdir(%s:%s) ==> SYSERR, couldn't find directory\n",
			path,xname);
#endif
		pnfsclose(pnfs);
		freemem(pnfs,sizeof(struct nfsblk));
		return(SYSERR);
	}

	/* make sure there's nothing there */
	if (pnfs->nfs_fhandle_valid) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsmkdir(%s:%s) ==> SYSERR, file exists\n",
			path,xname);
#endif
		pnfsclose(pnfs);
		freemem(pnfs,sizeof(struct nfsblk));
		return(SYSERR);
	}

	/* use the RPC device from the mount table entry */
	dev = pnfs->nfs_pme->mnt_rpcdev;
	wait(pnfs->nfs_pme->mnt_mutex);

	/* just make the call */
	retval = nfscall(dev, NFSPROC_MKDIR, &pnfs->nfs_status, pnfs->nfs_pme,
			 "%F %S {8%U}",
			 pnfs->nfs_phandle, NFS_FHSIZE,
			 pnfs->nfs_fname, 0,
			 &mkdirsattr.sa_mode);

	signal(pnfs->nfs_pme->mnt_mutex);
	status = pnfs->nfs_status;
	pnfsclose(pnfs);
	freemem(pnfs,sizeof(struct nfsblk));

	if (retval == SYSERR || status != NFS_OK) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsmkdir: nfsCall returns %d\n", retval);
#endif
		return(SYSERR);
	}
	
	return(OK);
}



/*
 * ====================================================================
 * nfsrmdir - try to remove an NFS directory
 * ====================================================================
 */
nfsrmdir(path)
     char *path;
{
	struct nfsblk *pnfs;
	int status;
	char xname[NAMLEN];
	int retval;

#ifdef DEBUG
	kprintf("nfsrmdir(%s) called\n", path);
#endif

	/* run the name through the namespace */
	if (nammap(path,xname) != NFS) {
		/* couldn't be an NFS file */
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsrmdir(%s) ==> SYSERR, not an NFS file\n", path);
#endif
		return(SYSERR);
	}

	/* initialize the nfsblk struct */
	pnfs = (struct nfsblk *) getmem(sizeof(struct nfsblk));
	pnfsopen(pnfs);

	/* look up the file */
	if ((retval = nfsfindfile(pnfs,xname,NFS_MUST_EXIST,TRUE)) == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsrmdir(%s) ==> SYSERR, no such directory\n", path);
#endif
		pnfsclose(pnfs);
		freemem(pnfs,sizeof(struct nfsblk));
		return(SYSERR);
	}

	/* make sure it's a directory */
	if (pnfs->nfs_ftype != NFDIR) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsrmdir(%s) ==> SYSERR, not a directory\n", path);
#endif
		pnfsclose(pnfs);
		freemem(pnfs,sizeof(struct nfsblk));
		return(SYSERR);
	}
	
	/* try to remove it */
	wait(pnfs->nfs_pme->mnt_mutex);
	retval = nfscall(pnfs->nfs_pme->mnt_rpcdev, NFSPROC_RMDIR,
			 &pnfs->nfs_status, pnfs->nfs_pme,
			 "%F %S",
			 pnfs->nfs_phandle, NFS_FHSIZE,	/* parent handle */
			 pnfs->nfs_fname, 0);		/* file name */
	signal(pnfs->nfs_pme->mnt_mutex);
	status = pnfs->nfs_status;
	pnfsclose(pnfs);
	freemem(pnfs,sizeof(struct nfsblk));

	if ((retval == SYSERR) || (status != NFS_OK)) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsrmdir: nfscall returns %d\n", retval);
#endif
		return(SYSERR);
	}

	return(OK);
}
