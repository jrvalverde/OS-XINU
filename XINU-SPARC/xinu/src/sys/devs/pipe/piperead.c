#include <conf.h>
#include <kernel.h>
#include <pipe.h>
#include <io.h>

piperead(devptr, buf, count)
	struct devsw *devptr;
	char *buf;
	int count;
{
	register struct pipe *pp;
	register int ccount;

	disable();

	pp = (struct pipe *) devptr->dvioblk;

	if (0 == count) /* read all available */
		count = pipe_count(pp);

	while(count) {
		wait(pp->i_readsem);
		*buf++ = pp->buffer[pp->i_nextout];
		pipe_bump(pp->i_nextout);
	}

	restore();
	return count;
}
