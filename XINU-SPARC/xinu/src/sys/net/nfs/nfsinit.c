/* nfsinit.c - nfsinit */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rpc.h>
#include <nfs.h>


/*
 * ====================================================================
 * nfsinit - initialize a single nfs pseudo device given the pointer
 *           to its device control block
 * ====================================================================
 */
nfsinit(pdev)
     struct devsw *pdev;
{
	struct	nfsblk	*pnfs;

	pnfs = &nfstab[pdev->dvminor];

	pdev->dvioblk = (char *) pnfs;

	pnfs->nfs_state = NFS_STAT_FREE;
	pnfs->nfs_dnum = pdev->dvnum;

	return(OK);
}
