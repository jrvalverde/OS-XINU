#include <conf.h>
#include <kernel.h>
#include <pipe.h>
#include <io.h>
pipegetc(devptr)
	struct devsw *devptr;
{
	char data;
	return piperead(devptr, &data, 1);
}
