#include <conf.h>
#include <kernel.h>
#include <pipe.h>
#include <sem.h>
#include <io.h>
pipeopen(devptr)
	struct devsw *devptr;
{
	register struct pipe *pptr;
	STATWORD ps;

	disable(ps);
	pptr = (struct pipe *) devptr->dvioblk;
	if (PIPE_FREE == pptr->mode) {
		pptr->mode = PIPE_INUSE;
		pptr->i_nextout = pptr->i_nextin = 0;
		sreset(pptr->i_readsem, pipe_count(pptr));
		sreset(pptr->i_writesem, pipe_space(pptr));
		restore(ps);
		return devptr->dvnum;
	}
	restore(ps);
	return SYSERR;
}
