#include <conf.h>
#include <kernel.h>
#include <pipe.h>
#include <io.h>


pipeclose(devptr)
	struct devsw *devptr;
{
	register struct pipe *pptr;
	STATWORD ps;

	disable(ps);
	pptr = (struct pipe *) devptr->dvioblk;
	pptr->mode = PIPE_FREE;

	restore(ps);
	
	
}
