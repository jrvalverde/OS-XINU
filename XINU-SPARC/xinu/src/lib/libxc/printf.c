/* printf.c - printf */

#define	OK	1
#define	CONSOLE	0

#include <varargs.h>

/*------------------------------------------------------------------------
 *  printf  --  write formatted output on CONSOLE 
 *------------------------------------------------------------------------
 */
/*VARARGS1*/
printf(fmt, va_alist)
     char	*fmt;
     va_dcl
{
	int	putc();
	va_list	ap;

	va_start(ap);

	_doprnt(fmt, ap, putc, CONSOLE);
	va_end(ap);

	return(OK);
}
