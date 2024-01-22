/* kprintf.c - kprintf */

#include <conf.h>
#include <kernel.h>
#include <tty.h>

/*------------------------------------------------------------------------
 *  kprintf  --  kernel printf: formatted, unbuffered output to CONSOLE
 *------------------------------------------------------------------------
 */
int
kprintf(fmt, args)
char	*fmt;
int	args;
{
	STATWORD	ps;
	unsigned int	saveof;
        int		kputc();

	disable(ps);
	saveof = control(CONSOLE, TTC_GOF);
	control(CONSOLE, TTC_SYNC, 1);
        _doprnt(fmt, &args, kputc, CONSOLE);
	if ((saveof & TOF_SYNC) == 0)
		control(CONSOLE, TTC_SYNC, 0);
	restore(ps);
        return OK;
}
