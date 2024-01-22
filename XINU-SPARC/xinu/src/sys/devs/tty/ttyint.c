/* ttyint.c - zilog chip (serial line) interrupt handle  */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <slu.h>
#include <zsreg.h>

/*#define DEBUG*/


/*------------------------------------------------------------------------
 *  ttyint  --  handle tty interrupt and demux to ttyiin() or ttyoin()
 *------------------------------------------------------------------------
 */
INTPROC	ttyint()
{
	STATWORD ps;    
	register int	ch;
	struct	tty   *iptr;

#ifdef DEBUG
	kprintf("in ttyint\n");
#endif
	iptr = &tty[0];			/* assume tty[0] - kinda bogus */

	/* Check for break */
	disable(ps);

	/* get contents of register 0 */
	(iptr->ioaddr)->zscc_control = 0;
	ch = (iptr->ioaddr)->zscc_control;

	/* check for a break - call ttybreak() if break detected */
	if ( ch & ZSRR0_BREAK ) {
	    ttybreak();
	}

	/* Test for input or output interrupt */

	/* get contents of register 3 */
	(iptr->ioaddr)->zscc_control = 3;
	ch = (iptr->ioaddr)->zscc_control;

	if ( ch & ZSRR3_IP_A_TX ) {	/* transmit */
		ttyoin(&(tty[0]));	/* assume tty[0] - kinda bogus */
	}
	if ( ch & ZSRR3_IP_A_RX ) {	/* receive */
		ttyiin(&(tty[0]));	/* assume tty[0] - kinda bogus */
	}

	(iptr->ioaddr)->zscc_control = ZSWR0_CLR_INTR; /* reset interrupt */
	restore(ps);			/* restore status register	*/
}
