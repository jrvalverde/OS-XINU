/* vcuiin.c --  vcuiin, verase1, veputc, vechoch */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <vcu.h>
#include <procreg.h>

/*------------------------------------------------------------------------
 *  vcuiin  --  lower-half vcu device driver for input interrupts
 *------------------------------------------------------------------------
 */
INTPROC	vcuiin(iptr)
	register struct	tty	*iptr;	/* pointer to tty block		*/
{
	register int	ch, ct;

	ch = mfpr(RXDB) & VCUICHMASK;		/* read char from device*/
	if (ch & VCURERMASK)
		return;				/* discard if error	*/
	if (iptr->imode == IMRAW) {
		if (scount(iptr->isem) >= IBUFLEN) {
			return;			/* discard if no space	*/
		}
		iptr->ibuff[iptr->ihead++] = ch;
		if (iptr->ihead	>= IBUFLEN)	/* wrap buffer pointer	*/
			iptr->ihead = 0;
	        signal(iptr->isem);
	} else {				/* cbreak | cooked mode	*/
		if ( ch	== RETURN && iptr->icrlf )
			ch = NEWLINE;
		if (iptr->iintr && ch == iptr->iintrc) {
			send(iptr->iintpid, INTRMSG);
			veputc(ch, iptr);
			return;
		}
		if (iptr->oflow) {
			if (ch == iptr->ostart)	{
				iptr->oheld = FALSE;
				mtpr(VCUTXCSENBL, TXCS);
				return;
			}
			if (ch == iptr->ostop) {
				iptr->oheld = TRUE;
				return;
			}
		}
		iptr->oheld = FALSE;
		if (iptr->imode	== IMCBREAK) {		/* cbreak mode	*/
			if (scount(iptr->isem) >= IBUFLEN) {
				veputc(iptr->ifullc,iptr);
				return;
			}
			iptr->ibuff[iptr->ihead++] = ch;
			if (iptr->ihead	>= IBUFLEN)
				iptr->ihead = 0;
			if (iptr->iecho)
				vechoch(ch,iptr);
			if (scount(iptr->isem) < IBUFLEN)
				signal(iptr->isem);
		} else {				/* cooked mode	*/
			if (ch == iptr->ikillc && iptr->ikill) {
				iptr->ihead -= iptr->icursor;
				if (iptr->ihead	< 0)
					iptr->ihead += IBUFLEN;
				iptr->icursor =	0;
				veputc(RETURN,iptr);
				veputc(NEWLINE,iptr);
				return;
			}
			if (ch == iptr->ierasec	&& iptr->ierase) {
				if (iptr->icursor > 0) {
					iptr->icursor--;
					verase1(iptr);
				}
				return;
			}
			if (ch == NEWLINE || ch == RETURN ||
				(iptr->ieof && ch == iptr->ieofc)) {
				if (iptr->iecho) {
					vechoch(ch,iptr);
					if (ch == iptr->ieofc)
					vechoch(NEWLINE,iptr);
				}
				iptr->ibuff[iptr->ihead++] = ch;
				if (iptr->ihead	>= IBUFLEN)
					iptr->ihead = 0;
				ct = iptr->icursor+1; /* +1 for \n or \r*/
				iptr->icursor =	0;
				signaln(iptr->isem,ct);
				return;
			}
			ct = scount(iptr->isem);
			ct = ct	< 0 ? 0	: ct;
			if ((ct	+ iptr->icursor) >= IBUFLEN-1) {
				veputc(iptr->ifullc,iptr);
				return;
			}
			if (iptr->iecho)
				vechoch(ch,iptr);
			iptr->icursor++;
			iptr->ibuff[iptr->ihead++] = ch;
			if (iptr->ihead	>= IBUFLEN)
				iptr->ihead = 0;
		}
	}
}

/*------------------------------------------------------------------------
 *  verase1  --  erase one character honoring erasing backspace
 *------------------------------------------------------------------------
 */
LOCAL verase1(iptr)
	struct	tty	*iptr;
{
	char	ch;

	if (--(iptr->ihead) < 0)
		iptr->ihead += IBUFLEN;
	ch = iptr->ibuff[iptr->ihead];
	if (iptr->iecho) {
		if (ch < BLANK || ch == 0177) {
			if (iptr->evis)	{
				veputc(BACKSP,iptr);
				if (iptr->ieback) {
					veputc(BLANK,iptr);
					veputc(BACKSP,iptr);
				}
			}
			veputc(BACKSP,iptr);
			if (iptr->ieback) {
				veputc(BLANK,iptr);
				veputc(BACKSP,iptr);
			}
		} else {
			veputc(BACKSP,iptr);
			if (iptr->ieback) {
				veputc(BLANK,iptr);
				veputc(BACKSP,iptr);
			}
		}
	} else
		mtpr (VCUTXCSENBL, TXCS);
}

/*------------------------------------------------------------------------
 *  vechoch  --  echo a character with visual and ocrlf options
 *------------------------------------------------------------------------
 */
LOCAL vechoch(ch, iptr)
	char	ch;			/* character to	echo		*/
	register struct	tty	*iptr;	/* ptr to I/O block for this dev*/
{
	if ((ch==NEWLINE||ch==RETURN)&&iptr->ecrlf) {
		veputc(RETURN,iptr);
		veputc(NEWLINE,iptr);
	} else if ((ch<BLANK||ch==0177) && iptr->evis) {
		veputc(UPARROW,iptr);
		veputc(ch+0100,iptr);/* make it printable	*/
	} else {
		veputc(ch,iptr);
	}
}

/*------------------------------------------------------------------------
 *  veputc - put one character in the echo queue
 *------------------------------------------------------------------------
 */
LOCAL veputc(ch,iptr)
	char	ch;
	register struct	tty	*iptr;
{
	iptr->ebuff[iptr->ehead++] = ch;
	if (iptr->ehead	>= EBUFLEN)
		iptr->ehead = 0;
	mtpr(VCUTXCSENBL, TXCS);
}
