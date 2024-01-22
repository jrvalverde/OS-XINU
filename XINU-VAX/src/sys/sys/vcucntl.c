/* vcucntl.c - vcucntl */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <vcu.h>

/*------------------------------------------------------------------------
 *  vcucntl  -  control the vax console unit device by setting modes
 *------------------------------------------------------------------------
 */
vcucntl(devptr, func, addr)
struct	devsw	*devptr;
int func;
char *addr;
{
	register struct	tty *ttyp;
	char	ch;
	PStype	ps;

	ttyp = &tty[devptr->dvminor];
	switch ( func )	{

	case TCNEXTC:
		disable(ps);
		wait(ttyp->isem);
		ch = ttyp->ibuff[ttyp->itail];
		restore(ps);
		signal(ttyp->isem);
		return(ch);
	case TCMODER:
		ttyp->imode = IMRAW;
		break;
	case TCMODEC:
		ttyp->imode = IMCOOKED;
		break;
	case TCMODEK:
		ttyp->imode = IMCBREAK;
		break;
	case TCECHO:
		ttyp->iecho = TRUE;
		break;
	case TCNOECHO:
		ttyp->iecho = FALSE;
		break;
	case TCICHARS:
		return(scount(ttyp->isem));
	case TCINT:
		ttyp->iintr = TRUE;
		ttyp->iintpid = getpid();
		break;
	case TCNOINT:
		ttyp->iintr = FALSE;
		break;
	default:
		return(SYSERR);
	}
	return(OK);
}
