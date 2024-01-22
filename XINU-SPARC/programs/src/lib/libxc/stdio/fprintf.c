/* fprintf.c - fprintf */

#include <stdio.h>
#include <varargs.h>

/*------------------------------------------------------------------------
 * fprintf  --  print a formatted message to a file
 *------------------------------------------------------------------------
 */
fprintf(iobptr, fmt, va_alist)
FILE *iobptr;
char *fmt;
va_dcl
{
        int     xputc();
	va_list	ap;

	va_start(ap);

        _doprnt(fmt, ap, xputc, readdtable(iobptr->_file));

	va_end(ap);

	return(1);		/* not correct, but will have to do */
}

int
vfprintf(iop, fmt, ap)
	FILE *iop;
	char *fmt;
	va_list ap;
{
	int len;

	len = _doprnt(fmt, ap, xputc, readdtable(iop->_file));
	return(1);
}
