/****************************************/
/*					*/
/*	Modified from original source:	*/
/*					*/
/*	swait, ssignal added		*/
/*					*/
/****************************************/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)putw.c	5.2 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

#include <stdio.h>

putw(w, iop)
register FILE *iop;
{
    int ret_val;
	register char *p;
	register i;

    stdio_swait(iop);

	p = (char *)&w;
	for (i=sizeof(int); --i>=0;)
		stdio_putc(*p++, iop);
    ret_val = (stdio_ferror(iop));

    stdio_ssignal(iop);

    return(ret_val);
}
