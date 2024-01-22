/* x_bpool.c - x_bpool */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <bufpool.h>

/*------------------------------------------------------------------------
 *  x_bpool  -  (command bpool) format and print buffer pool information
 *------------------------------------------------------------------------
 */
COMMAND	x_bpool(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	struct	bpool	*bpptr;
	char	str[80];
	int	i;

	for (i=1 ; i<nbpools ; i++) {
		bpptr = &bptab[i];
		sprintf(str,
		    "pool=%2d. bsize=%6d, sem=%3d, count=%d\n",
			i, bpptr->bpsize, bpptr->bpsem,
			scount(bpptr->bpsem));
		write(stdout, str, strlen(str));
#ifdef BUF_DEBUG
		bpcheck(i);
#endif
	}
	return(OK);
}
