/* x_nfs.c - x_nfs */


#include <conf.h>
#include <kernel.h>
#include <proc.h>
#ifdef Nnfs
#include <network.h>
#include <portmapper.h>
#include <rpc.h>
#include <rpcprogs.h>
#include <nfs.h>
#include <mount.h>
#endif

#define DEBUG


static mount();



/*------------------------------------------------------------------------
 *  x_nfs  -  dump nfs information
 *------------------------------------------------------------------------
 */
COMMAND	x_nfs(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
#ifdef Nnfs
	int i;
	struct mountentry *pme;
	struct nfsblk *pnfs;

	fprintf(stdout,"mountTableMutex: %d\n", mountTableMutex);
	fprintf(stdout,"\nMount table:\n");
	fprintf(stdout,"host        fs          RTO    mutex  rdev  gen  openct\n");
	fprintf(stdout,"==========  ==========  =====  =====  ====  ===  ======\n");
	for (i=0; i < MOUNTTABLESIZE; ++i) {
		pme = &mounttab[i];
		if (pme->mnt_state == RPCMOUNT_STATUS_FREE)
		    continue;
		if (pme->mnt_state == RPCMOUNT_STATUS_RESERVED) {
			fprintf(stdout,"RESERVED\n");
			continue;
		}
		fprintf(stdout,"%-10s  ", pme->mnt_host);
		fprintf(stdout,"%-10s  ", pme->mnt_fs);
		fprintf(stdout,"%5d  ", pme->mnt_timeout);
		fprintf(stdout,"%5d  ", pme->mnt_mutex);
		fprintf(stdout,"%4d  ", pme->mnt_rpcdev);
		fprintf(stdout,"%3d  ", pme->mnt_generation);
		fprintf(stdout,"%6d  ", pme->mnt_opencount);
		fprintf(stdout,"\n");
	}

	
	fprintf(stdout,"\nNFS table:\n");
	fprintf(stdout,"dev  rdev  mode  fpos      host        fs          file\n");
	fprintf(stdout,"===  ====  ====  ========  ==========  ==========  =========\n");
	for (i=0; i < Nnfs; ++i) {
		pnfs = &nfstab[i];
		if (pnfs->nfs_state == NFS_STAT_FREE)
		    continue;
		fprintf(stdout,"%3d  ", pnfs->nfs_dnum);
		fprintf(stdout,"%4d  ", pnfs->nfs_rpcdev);
		fprintf(stdout,"%c%c%c%c  ",
			(pnfs->nfs_mode & FLREAD)?'r':'-',
			(pnfs->nfs_mode & FLWRITE)?'w':'-',
			(pnfs->nfs_mode & FLOLD)?'o':'-',
			(pnfs->nfs_mode & FLNEW)?'n':'-');
		fprintf(stdout,"%8d  ", pnfs->nfs_fpos);
		fprintf(stdout,"%-10s  ", pnfs->nfs_pme->mnt_host);
		fprintf(stdout,"%-10s  ", pnfs->nfs_pme->mnt_fs);
		fprintf(stdout,"%-10s  ", pnfs->nfs_fpath);
		fprintf(stdout,"\n");
	}
#endif
}
