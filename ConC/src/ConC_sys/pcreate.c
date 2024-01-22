/* pcreate.c - pcreate */

#include <kernel.h>
#include <ports.h>
#include <errno.h>

/*------------------------------------------------------------------------
 *  pcreate  --  create a port that allows "count" outstanding messages
 *------------------------------------------------------------------------
 */
SYSCALL	pcreate(count)
int	count;
{
	long	ps;
	int	i, p;
	struct	pt	*ptptr;

	if (count < 0) {
		errno = EINVAL;
		return(SYSERR);
		}

	disable(ps);
	if ( !ptmark )
		pinit(MAXMSGS);
	for (i=0 ; i<NPORTS ; i++) {
		if ( (p=ptnextp--) <= 0)
			ptnextp = NPORTS - 1;
		if ( (ptptr= &ports[p])->ptstate == PTFREE) {
			ptptr->ptstate = PTALLOC;
			ptptr->ptssem = screate(count);
			ptptr->ptrsem = screate(0);
			ptptr->pthead = ptptr->pttail = (struct ptnode *)NULL;
			ptptr->ptseq++;
			ptptr->ptmaxcnt = count;
			restore(ps);
			return(p);
		}
	}
	restore(ps);
	errno = CENOPORT;
	return(SYSERR);
}
