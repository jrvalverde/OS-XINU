#include <conf.h>
#include <kernel.h>
#include <pipe.h>
#include <io.h>
pipeputc(devptr, ch)
	struct devsw *devptr;
	char ch;
{
	return pipewrite(devptr, &ch, 1);
}
