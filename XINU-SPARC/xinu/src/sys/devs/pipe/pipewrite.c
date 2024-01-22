#include <conf.h>
#include <kernel.h>
#include <pipe.h>
#include <io.h>
pipewrite(devptr, buf, count)
	struct devsw *devptr;
	char *buf;
	int count;
{
	register struct pipe *pp;
	register int ccount;

	disable();

	pp = (struct pipe *) devptr->dvioblk;
	while(count) {
		wait(pp->i_writesem);
		pp->buffer[pp->i_nextin] = *buf++;
		pipe_bump(pp->i_nextin);
	}

	restore();
	return count;
}
