/* x_df.c - x_df */


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

#ifndef Nnfs
COMMAND x_df()
{
	return(SYSERR);
}
#else Nnfs


static PrintOne();
static PrintForFile();


/* structure used for return of RPC call */
static struct fstat {
	unsigned tsize;
	unsigned bsize;
	unsigned blocks;
	unsigned bfree;
	unsigned bavail;
};

#define BYTESPERK 1024

int stdin, stdout, stderr;

/*------------------------------------------------------------------------
 *  x_df  -  Xinu version of the Unix df command
 *------------------------------------------------------------------------
 */
COMMAND	x_df(stdin_in, stdout_in, stderr_in, nargs, args)
int	stdin_in, stdout_in, stderr_in, nargs;
char	*args[];
{
	int i;
	struct mountentry *pme;

	stdin = stdin_in;
	stdout = stdout_in;
	stderr = stderr_in;

	fprintf(stdout,"Filesystem              kbytes      used     avail  capacity\n");
	fprintf(stdout,"====================  ========  ========  ========  ========\n");
	if (nargs == 1) {
		for (i=0; i < MOUNTTABLESIZE; ++i) {
			pme = &mounttab[i];
			if (pme->mnt_state == RPCMOUNT_STATUS_VALID)
			    PrintOne(pme);
		}
	} else {
		for (i=1; i < nargs; ++i)
		    PrintForFile(args[i]);
	}
}


static
PrintForFile(path)
     char *path;
{
	struct nfsblk nfs, *pnfs = &nfs;
	char newname[100];

	/* initialize the nfsblk struct */
	pnfsopen(pnfs);
	
	if ((nammap(path,newname) == SYSERR) ||
	    (nfsfindfile(pnfs,newname,NFS_MUST_EXIST) == SYSERR))
	    fprintf(stderr,"%s: not found\n", path);
	else
	    PrintOne(pnfs->nfs_pme);

	pnfsclose(pnfs);
}


static
PrintOne(pme)
     struct mountentry *pme;
{
	unsigned status;
	struct fstat fstat;
	int dev;
	int bused;
	char name[100];

	dev = pme->mnt_rpcdev;
	sprintf(name,"%s:%s", pme->mnt_host, pme->mnt_fs);
	fprintf(stdout,"%-20s  ", name);

	if ((control(dev,RPC_SETPROC,NFSPROC_STATFS) == SYSERR) ||
	    (rpcprintf(dev, "%F", pme->mnt_handle, NFS_FHSIZE) == SYSERR) ||
	    (control(dev,RPC_CALL) == SYSERR) ||
	    (control(dev,XGET_UINT,&status) == SYSERR) ||
	    (status != NFS_OK)) {
		fprintf(stdout,"UNAVAILABLE\n");
		return;
	}
	rpcscanf(dev,"{5%U}",&fstat);
	bused = fstat.blocks - fstat.bfree;
	fprintf(stdout,"%8d  ", (fstat.bsize * fstat.blocks) / BYTESPERK);
	fprintf(stdout,"%8d  ", (bused * fstat.bsize) / BYTESPERK);
	fprintf(stdout,"%8d  ", (fstat.bavail  * fstat.bsize) / BYTESPERK);
	fprintf(stdout,"  %2d%%  ",
		(100 * (fstat.blocks - fstat.bavail)) / fstat.blocks);
	fprintf(stdout,"\n");
}

#endif Nnfs
