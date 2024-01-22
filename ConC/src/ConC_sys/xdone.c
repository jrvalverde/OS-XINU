/* xdone.c - xdone */

#include <kernel.h>

/*------------------------------------------------------------------------
 * xdone  --  print system completion message as last process exits
 *------------------------------------------------------------------------
 */
xdone()
{
	halt();		/* halt is exit(0) */
}
