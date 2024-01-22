/* ovflon.c -- ovflon */

#include <kernel.h>

/*------------------------------------------------------------------------
 * ovflon -- turn on checking for integer overflow
 *------------------------------------------------------------------------
 */

SYSCALL ovflon()
{
	asm ("movpsl	r0");		/* return old psl value, set saved */
	asm ("bisl2	$0x20, 4(fp)");	/* psl int overflow enable bit (on */
}					/* stack frame). New psl popped by */
					/* ret.				   */
