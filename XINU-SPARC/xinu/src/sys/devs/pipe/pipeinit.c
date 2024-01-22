#include <conf.h>
#include <kernel.h>
#include <pipe.h>
#include <io.h>

struct pipe pipe[1];

pipeinit(devptr)
	struct devsw *devptr;
{
	struct pipe *pptr;

	pptr = &pipe[0];
	devptr->dvioblk = (char *) pptr;

	pptr->mode = PIPE_FREE;
	pptr->i_nextin = 0;
	pptr->i_nextout = 0;

	if (SYSERR == (pptr->i_readsem = screate(pipe_count(pptr)))) {
		kprintf("failed to allocate semaphore for pipe.\n");
		return SYSERR;
	}
	if (SYSERR == (pptr->i_writesem = screate(pipe_count(pptr)))) {
		kprintf("failed to allocate semaphore for pipe.\n");
		return SYSERR;
	}
	
}

