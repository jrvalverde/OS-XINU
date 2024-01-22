/* nullpcb.c */

#include <kernel.h>
#include <proc.h>

			/* nullpcb is the location of the null process' */
			/* process control block within proctab */

char *nullpcb = (char *) &(proctab[NULLPROC].pregs[0]);
