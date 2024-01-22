/* vcuinit.c - vcuinit */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <tty.h>
#include <io.h>
#include <vcu.h>
#include <procreg.h>

/*------------------------------------------------------------------------
 *  vcuinit - initialize buffers and modes for a vax console device
 *------------------------------------------------------------------------
 */
vcuinit(devptr)
	struct	devsw	*devptr;
{
	register struct	tty *iptr;
	int	 junk, isconsole;

	/* set up interrupt vector and interrupt dispatch table */

	iptr = &tty[devptr->dvminor];
	iosetvec(devptr->dvnum, iptr, iptr);

	devptr->dvioblk = (char *)iptr;		/* fill tty control blk	*/
	isconsole = (devptr->dvnum == CONSOLE);	/* make console cooked	*/
	iptr->ioaddr=(struct csr *)devptr->dvcsr;/* not necessary	*/
	iptr->ihead = iptr->itail = 0;		/* empty input queue	*/
	iptr->isem = screate(0);		/* chars. read so far=0	*/
	iptr->osem = screate(OBUFLEN);		/* buffer available=all	*/
	iptr->odsend = 0;			/* sends delayed so far	*/
	iptr->ohead = iptr->otail = 0;		/* output queue empty	*/
	iptr->ehead = iptr->etail = 0;		/* echo queue empty	*/
	iptr->imode = (isconsole ? IMCOOKED : IMRAW);
	iptr->iecho = iptr->evis = isconsole;	/* echo console input	*/
	iptr->ierase = iptr->ieback = isconsole;/* console honors erase	*/
	iptr->ierasec = BACKSP;			/*  using ^h		*/
	iptr->ecrlf = iptr->icrlf = isconsole;	/* map RETURN on input	*/
	iptr->ocrlf = iptr->oflow = isconsole;	/* map RETURN on output	*/
	iptr->ieof  = iptr->ikill = isconsole;	/* set line kill=KILLCH */
	iptr->iintr = FALSE;
	iptr->iintrc = INTRCH;
	iptr->iintpid = BADPID;
	iptr->ikillc = KILLCH;
	iptr->ieofc = EOFC;
	iptr->oheld = FALSE;
	iptr->ostart = STRTCH;
	iptr->ostop = STOPCH;
	iptr->icursor = 0;
	iptr->ifullc = TFULLC;
	junk = mfpr(RXDB);			/* clear receiver */
	mtpr(VCURXCSENBL, RXCS);		/* enable in intrpts*/
	mtpr(VCUTXCSDSBL, TXCS);		/* disable out "  */
}
