/* vcuputc.c - vcuputc */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <vcu.h>
#include <procreg.h>

/*------------------------------------------------------------------------
 *  vcuputc - write one character to vax console device
 *------------------------------------------------------------------------
 */
vcuputc(devptr, ch )
struct	devsw	*devptr;
char	ch;
{
	struct	tty   *iptr;
	PStype	ps;

	iptr = &tty[devptr->dvminor];
        if ( ch==NEWLINE && iptr->ocrlf )
                vcuputc(devptr,RETURN);
	disable(ps);
	wait(iptr->osem);		/* wait	for space in queue	*/
	iptr->obuff[iptr->ohead++] = ch;
	if (iptr->ohead	>= OBUFLEN)
		iptr->ohead = 0;
	mtpr(VCUTXCSENBL, TXCS);
	restore(ps);
	return(OK);
}
