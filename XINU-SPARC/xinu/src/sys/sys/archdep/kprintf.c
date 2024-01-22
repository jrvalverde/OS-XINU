/* kprintf.c - kprintf, kputc, savestate, rststate */

#include <conf.h>
#include <kernel.h>
#include <openprom.h>
#include <io.h>
#include <tty.h>

#include <varargs.h>			/* make is portable		*/

#define prputchar(x) (*romp->v_putchar)((unsigned char) x)

/*------------------------------------------------------------------------
 *  kprintf  --  kernel printf: formatted, unbuffered output to CONSOLE
 *------------------------------------------------------------------------
 */
/*VARARGS1*/
kprintf(fmt, va_alist)		/* Derived by Bob Brown, Purdue U.	*/
     char *fmt;
     va_dcl
{
    STATWORD ps;    
    int     kputc();
    va_list ap;
    
    disable(ps);
    va_start(ap);
    _doprnt(fmt, ap, kputc, CONSOLE);
    va_end(ap);
    restore(ps);
    return(OK);
}

/*------------------------------------------------------------------------
 *  kputc  --  write a character on the console using polled I/O
 *------------------------------------------------------------------------
 */
kputc(device, c)
     int	device;
     register unsigned c;	/* character to print from _doprnt	*/
{
    if ( c == 0 )
	return;
    if ( c > 127 ) {
	prputchar('M');		/* do what "cat -v" does */
	prputchar('-');		/* prom routine dies otherwise */
	prputchar((char) (c & 0x007f) );
	return;
    }
    
    if (c == NEWLINE)
	prputchar(RETURN);
    
    prputchar(c);
}
