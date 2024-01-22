/* ovfloff.c -- ovfloff */

#include <kernel.h>

/*------------------------------------------------------------------------
 * ovfloff -- turn off checking for integer overflow
 *------------------------------------------------------------------------
 */

SYSCALL ovfloff()
{
	asm ("movpsl	r0");		/* return old psl value, clear saved*/
	asm ("bicl2	$0x20, 4(fp)");	/* psl int overflow enable bit (on  */
}					/* stack frame). New psl popped by  */
					/* ret)				    */
