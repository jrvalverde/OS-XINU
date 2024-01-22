/* pinit.c - pinit */

#include <kernel.h>
#include <ports.h>
#include <mem.h>

struct	ptnode	*ptfree;		/* list of free queue nodes	*/
struct	pt	ports[NPORTS];
int	ptnextp;

/*------------------------------------------------------------------------
 *  pinit  --  initialize all ports
 *------------------------------------------------------------------------
 */
SYSCALL	pinit(maxmsgs)
int maxmsgs;
{
	int	i;
	struct	ptnode	*next, *prev;

	if ((int)(ptfree=(struct ptnode *)getmem(maxmsgs*sizeof(struct ptnode)))
								     == SYSERR )
		panic("pinit - insufficient memory");
	for (i=0 ; i<NPORTS ; i++) {
		ports[i].ptstate = PTFREE;
		ports[i].ptseq = 0;
	}
	ptnextp = NPORTS - 1;

	/* link up free list of message pointer nodes */

	for ( prev=next=ptfree ;  --maxmsgs > 0  ; prev=next )
		prev->ptnext = ++next;
	prev->ptnext = (struct ptnode *)NULL;
	ptmark = TRUE;

	return(OK);
}
