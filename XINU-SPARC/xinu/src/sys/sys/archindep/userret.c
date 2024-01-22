/* userret.c - userret */

#include <conf.h>
#include <kernel.h>


/*------------------------------------------------------------------------
 * userret  --  entered when a process exits by return
 *------------------------------------------------------------------------
 */
kernret()					/* for kernel process */
{
	kill( getpid() );
}

userret()					/* for user process */
{
	kill( getpid() );
}
