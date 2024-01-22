/* kbminput.c - kbminput */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

extern struct tty kbmtty;

/*------------------------------------------------------------------------
 * kbminput - handle an input character from the kbmsole
 *------------------------------------------------------------------------
 */
INTPROC kbminput(ch)
unsigned int	ch;
{
	struct tty	*iptr = &kbmtty;
	
kprintf("kbminput\n");
}
