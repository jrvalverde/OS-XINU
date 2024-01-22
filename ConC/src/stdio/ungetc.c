/****************************************/
/*					*/
/*	Modified from original source:	*/
/*					*/
/*	swait, ssignal added		*/
/*					*/
/****************************************/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)ungetc.c	5.3 (Berkeley) 3/26/86";
#endif LIBC_SCCS and not lint

/*	ungetc.c	4.2	83/09/25	*/

#include <stdio.h>

ungetc(c, iop)
	register FILE *iop;
{

    stdio_swait(iop);
	if (c == EOF || (iop->_flag & (_IOREAD|_IORW)) == 0 ||
	    iop->_ptr == NULL || iop->_base == NULL)
		{stdio_ssignal(iop); return (EOF);}

	if (iop->_ptr == iop->_base)
		if (iop->_cnt == 0)
			iop->_ptr++;
		else
			{stdio_ssignal(iop); return (EOF);}

	iop->_cnt++;
	*--iop->_ptr = c;

    stdio_ssignal(iop);

	return (c);
}
