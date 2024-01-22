/* nfschar.c -- nfsputc, nfsgetc, nfsseek */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nfs.h>


/*#define DEBUG*/
#define PRINTERRORS


/*------------------------------------------------------------------------
 *  nfsseek  --  seek to a specified position of an NFS file
 *------------------------------------------------------------------------
 */
nfsseek(devptr, offset)
     struct	devsw	*devptr;
     unsigned	offset;
{
	struct	nfsblk	*pnfs;

	pnfs = (struct nfsblk *) devptr->dvioblk;

	wait(pnfs->nfs_mutex);
	pnfs->nfs_fpos = offset;
	signal(pnfs->nfs_mutex);

	return(OK);
}



/*------------------------------------------------------------------------
 *  nfsgetc  --  get a character from a remote file
 *------------------------------------------------------------------------
 */
nfsgetc(devptr)
struct	devsw	*devptr;
{
	int	ch;
	int	retval;

	/* nfsread will grab the mutex */
	retval = nfsread(devptr, &ch, 1);

	if (retval == 1)
	    return(ch);
	else
	    return(retval);
}



/*------------------------------------------------------------------------
 *  nfsputc  --  put a single character into a remote file
 *------------------------------------------------------------------------
 */
nfsputc(devptr, ch)
struct	devsw	*devptr;
int	ch;
{
	int	retval;

	/* nfswrite will grab the mutex */
	retval = nfswrite(devptr, &ch, 1);

	if (retval == 1)
	    return(OK);
	else
	    return(SYSERR);
}
