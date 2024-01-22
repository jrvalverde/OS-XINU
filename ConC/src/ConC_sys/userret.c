/* userret.c - userret */

#include <kernel.h>
#include <proc.h>

/*----------------------------------------------------------------------
 * userret - second simulated 'calls' frame on stack in do_create.
 *	     Does a tkill on itself.
 *----------------------------------------------------------------------
 */

userret()
{
	tkill(gettid());
}
