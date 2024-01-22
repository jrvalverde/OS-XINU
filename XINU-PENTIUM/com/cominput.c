/* cominput.c - cominput */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/*------------------------------------------------------------------------
 * cominput - handle an input character from a serial line device
 *------------------------------------------------------------------------
 */
INTPROC cominput(ch)
unsigned int	ch;
{
kprintf("cominput\n");
}
