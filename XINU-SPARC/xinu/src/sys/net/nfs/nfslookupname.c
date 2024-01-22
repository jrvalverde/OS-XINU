/* nfslookupname.c - nfslookupname */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>


/*#define DEBUG*/
#define PRINTERRORS


nfslookupname(pnfs, handle, name, rhandle, pattr, usecache)
     struct nfsblk *pnfs;
     fhandle handle;		/* parent handle */
     char *name;		/* name */
     fhandle rhandle;		/* return handle */
     struct fattr *pattr;	/* attributes */
     Bool usecache;		/* can we return a cached answer?? */
{
	int dev;
	int retval;

#ifdef DEBUG
	kprintf("nfslookupname(...,%s,...) called\n", name);
#endif

	/* check the fhandle cache */
	if (usecache && nfshclookup(pnfs->nfs_pme,
				    handle, name,
				    rhandle, pattr) == OK) {
		return(OK);
	}


	/* we use the RPC device from the mount table entry */
	dev = pnfs->nfs_pme->mnt_rpcdev;

	/* grab exclusive access to this device because it's shared	*/
	/* among all users on this mount point				*/
	wait(pnfs->nfs_pme->mnt_mutex);

	/* call the LOOKUP procedure */
	retval = nfscall(dev, NFSPROC_LOOKUP, &pnfs->nfs_status,
			 pnfs->nfs_pme,
			 "%F %S",
			 handle, NFS_FHSIZE,
			 name, 0);

	if (retval == SYSERR) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfslookupname: failed\n");
#endif
		signal(pnfs->nfs_pme->mnt_mutex);
		return(SYSERR);
	}

	if (pnfs->nfs_status == NFS_OK) {
		/* grab the new handle and file attributes */
		retval = rpcscanf(dev, "%F {17%U}",
				  rhandle, NFS_FHSIZE,
				  pattr);

		/* cache the answer */
		nfshcinsert(pnfs->nfs_pme,
			 handle, name,
			 rhandle, pattr);
	}

	signal(pnfs->nfs_pme->mnt_mutex);
	return(OK);
}
