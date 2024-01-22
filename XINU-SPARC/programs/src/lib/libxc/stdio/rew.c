/* rew.c - rewind() */

#include <stdio.h>

#ifdef MIPS
void
#endif
rewind(iop)
FILE *iop;
{
	lseek(fileno(iop), 0L, 0);
}
