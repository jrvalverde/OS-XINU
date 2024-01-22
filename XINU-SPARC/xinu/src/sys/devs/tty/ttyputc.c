/* ttyputc.c - ttyputc */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <slu.h>
#include <zsreg.h>

/*------------------------------------------------------------------------
 *  ttyputc - write one character to a tty device
 *------------------------------------------------------------------------
 */
ttyputc(devptr, ch)
struct	devsw	*devptr;
int	ch;
{
	STATWORD ps;    
	struct	tty   *iptr;

	iptr = &tty[devptr->dvminor];
        if (ch==NEWLINE && iptr->ocrlf )
                ttyputc(devptr, (char)RETURN);
	disable(ps);
	wait(iptr->osem);		/* wait	for space in queue	*/

	iptr->obuff[iptr->ohead++] = (char)ch;
	++iptr->ocnt;
	if (iptr->ohead	>= OBUFLEN)
		iptr->ohead = 0;
	ttyostart(iptr);
	restore(ps);
	return(OK);
}



