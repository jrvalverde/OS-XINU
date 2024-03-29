/* freemem.c - freemem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>

/*------------------------------------------------------------------------
 *  freemem  --  free a memory block, returning it to memlist
 *------------------------------------------------------------------------
 */
SYSCALL	freemem(block, size)
char *block;
word size;
{
	int	ps;
	struct	mblock	*p, *q;
	char	*top;

	size = roundew(size);
	block = (char *) truncew( (word)block );
	if ( size==0 || block > maxaddr || (maxaddr-block) < size ||
		block < end )
		return(SYSERR);
	disable(ps);
	(char *)q = NULL;
	for( p=memlist.mnext ;
		(char *)p != NULL && (char *)p < block ;
		q=p, p=p->mnext )
			;
	if ( (char *)q != NULL && (top=(char *)q+q->mlen) > block
		    || (char *)p != NULL && (block+size) > (char *)p ) {
		restore(ps);
		return(SYSERR);
	}
	if ( (char *)q != NULL && top == block )
		q->mlen += size;
	else {
		((struct mblock *)block)->mlen = size;
		((struct mblock *)block)->mnext = p;
		memlist.mnext = (struct mblock *)block;
		(char *)q = block;
	}
	/* note that q != NULL here */
	if ( (char *)p != NULL
		&& ((char *)q + q->mlen) == (char *)p) {
		q->mlen += p->mlen;
		q->mnext = p->mnext;
	}
	restore(ps);
	return(OK);
}
