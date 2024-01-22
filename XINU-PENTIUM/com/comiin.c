/* comiin.c comiin */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <com.h>

/*------------------------------------------------------------------------
 *  comiin  --  lower-half com device driver for input interrupts
 *------------------------------------------------------------------------
 */
INTPROC	comiin(pcom, ch)
struct comsoft	*pcom;
unsigned char	ch;
{
    struct devsw	*pdev = pcom->com_pdev;

    if (pdev->dvioblk == 0)
	return;		/* no tty device associated */
    
    pdev = (struct devsw *)pdev->dvioblk;	/* get tty dev pointer */
    
    if (pdev == 0)
	return;		/* no tty structure associated */
    
    (pdev->dviint)(pdev, ch);
}
