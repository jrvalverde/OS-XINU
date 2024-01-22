/* vcuoin.c - vcuoin */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <vcu.h>
#include <procreg.h>

/*------------------------------------------------------------------------
 *  vcuoin  --  lower-half vcu device driver for output interrupts
 *------------------------------------------------------------------------
 */
INTPROC	vcuoin(iptr)
	register	struct	tty	*iptr;
{
	register	int	ct;

	if (iptr->ehead	!= iptr->etail)	{
		mtpr(iptr->ebuff[iptr->etail++]&VCUOCHMASK, TXDB);
		if (iptr->etail	>= EBUFLEN)
			iptr->etail = 0;
		return;
	}
	if (iptr->oheld) {			/* honor flow control	*/
		mtpr(VCUTXCSDSBL, TXCS);
		return;
	}
	if ((ct=scount(iptr->osem)) < OBUFLEN) {
		mtpr(iptr->obuff[iptr->otail++]&VCUOCHMASK, TXDB);
		if (iptr->otail	>= OBUFLEN)
			iptr->otail = 0;
		if (ct > OBMINSP)
			signal(iptr->osem);
		else if	( ++(iptr->odsend) == OBMINSP) {
			iptr->odsend = 0;
			signaln(iptr->osem, OBMINSP);
		}
	} else
		mtpr(VCUTXCSDSBL, TXCS);
}
