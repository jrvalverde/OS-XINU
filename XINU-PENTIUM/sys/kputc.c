/* kputc.c - kputc */

#include <conf.h>
#include <kernel.h>
#include <tty.h>

extern int console_dev;

/*------------------------------------------------------------------------
 * kputc - do a synchronous kernel write to the console tty
 *------------------------------------------------------------------------
 */
int kputc(dev, c)
int		dev;	/* fake dev-- always the console */
unsigned char	c;
{
    if (console_dev == KBMON)
	kbmputc(&devtab[console_dev], c);
    else
	comsputc(&devtab[console_dev], c);
}
