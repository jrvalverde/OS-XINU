/* printf.c - printf */

#include <sys/xinusys.h>
#include <varargs.h>

#define DEVCONSOLE	readdtable(1)	/* unix output file descriptor	*/

/*------------------------------------------------------------------------
 *  printf  --  write formatted output on CONSOLE 
 *------------------------------------------------------------------------
 */
printf(fmt, va_alist)
     char	*fmt;
     va_dcl
{
	int	xputc();
	va_list	ap;

	va_start(ap);

	_doprnt(fmt, ap, xputc, DEVCONSOLE);

	va_end(ap);

	return(OK);
}
