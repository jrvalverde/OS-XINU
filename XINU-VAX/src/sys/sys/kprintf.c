/* kprintf.c - kprintf, kputc, savestate, rststate */

#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <slu.h>
#include <tty.h>
#include <procreg.h>

/*------------------------------------------------------------------------
 *  kprintf  --  kernel printf: formatted, unbuffered output to CONSOLE
 *------------------------------------------------------------------------
 */
kprintf(fmt, args)		/* Derived by Bob Brown, Purdue U.	*/
        char *fmt;		/* Flow control added by Steve Munson	*/
{
	int	kputc();
	savestate();
        _doprnt(fmt, &args, kputc, CONSOLE);
        rststate();
        return(OK);
}

/*------------------------------------------------------------------------
 *  kputc  --  write a character on the non memory-mapped console device
 *		 using polled I/O
 *------------------------------------------------------------------------
 */
LOCAL	kputc(device ,c)
	int	device;
	register char c;	/* character to print from _doprnt	*/
{
	register int 	crstat, crbuf, ctstat;
	register struct	tty	*ttyptr;

	if ( device != CONSOLE || c == 0 )
		return;
	if ( c == NEWLINE )
		kputc( device, RETURN );
	ttyptr = (struct tty *)devtab[device].dvioblk;	/* control block*/
	if (ttyptr) {
	   crstat = mfpr(RXCS); 			/* get recv. stat */
	   crbuf = mfpr(RXDB); 		/* get content of receiver buffer */
	   if ((crstat&SLUREADY) &&
		(ttyptr->oheld || (ttyptr->oflow &&
			(crbuf & SLUCHMASK) == ttyptr->ostop)))
			do {
			    crstat = mfpr(RXCS);	/* get recv stat */
			    while (!(crstat&SLUREADY)) /* wait til ready */
				    crstat = mfpr(RXCS);/* get recv stat */
			    crbuf = mfpr(RXDB); 	/* get rec buffer */
			} while ((crbuf&SLUCHMASK) == ttyptr->ostop);
			ttyptr->oheld = FALSE;
	   }
	ctstat = mfpr(TXCS); /* get transmit status */
	while (!(ctstat & SLUREADY)) /* not ready to transmit yet */
		ctstat = mfpr(TXCS); /* poll for idle*/
	mtpr(c & SLUCHMASK, TXDB); /* transmit character */
	ctstat = mfpr(TXCS); /* get transmit status */
	while (!(ctstat & SLUREADY))
		ctstat = mfpr(TXCS); /* get transmit status */
}

LOCAL	int	savecrstat, savectstat;
LOCAL	PStype	ps;
/*------------------------------------------------------------------------------
 *  savestate  --  save the state of the non-memory mapped CONSOLE device
 *------------------------------------------------------------------------------
 */
LOCAL	savestate()
{
	disable(ps);
	savecrstat = mfpr(RXCS) & SLUENABLE;
	savectstat = mfpr(TXCS) & SLUENABLE;
	mtpr(SLUDISABLE, RXCS); 		/* disable receive intpts  */
	mtpr(SLUDISABLE, TXCS);			/* disable transmit intpts */
}

/*--------------------------------------------------------------------------
 *  rststate  --  restore the state of the non-memory mapped CONSOLE device
 *--------------------------------------------------------------------------
 */
LOCAL	rststate()
{
	mtpr(savecrstat, RXCS);
	mtpr(savectstat, TXCS);
	restore(ps);
}
