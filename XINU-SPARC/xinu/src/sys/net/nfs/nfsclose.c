/* nfsclose.c - nfsclose */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>
#include <mount.h>


/*#define DEBUG*/
#define PRINTERRORS


/*------------------------------------------------------------------------
 *  nfsclose  --  close an nfs file
 *------------------------------------------------------------------------
 */
nfsclose(devptr)
struct	devsw	*devptr;
{
	struct	nfsblk	*pnfs;

	pnfs = &nfstab[devptr->dvminor];

	wait(pnfs->nfs_mutex);

	/* make sure the device is in use */
	if (pnfs->nfs_state != NFS_STAT_USED) {
#if defined(DEBUG) || defined(PRINTERRORS)
		kprintf("nfsclose: file wasn't open\n");
#endif
		signal(pnfs->nfs_mutex);
		return(SYSERR);
	}

	--pnfs->nfs_pme->mnt_opencount;
	nfsdealloc(pnfs);
	signal(pnfs->nfs_mutex);
	return(OK);
}
