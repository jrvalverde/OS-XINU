/* dprintf.c - dprintf */

#include <conf.h>
#include <kernel.h>

dprintf(dev,m,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)
     int dev;
     char *m;
     int a1,a2,a3,a4,a5,a6,a7,a8,a9,a10;
{
	if (dev < 0)
	    kprintf(m,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);
	else 
	    fprintf(dev,m,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);
}

