/* init.c - init_dev */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  init_dev  -  initialize a device
 *------------------------------------------------------------------------
 */
init_dev(descrp)
int descrp;
{
    struct	devsw	*pdev;

    if (isbaddev(descrp) ) {
	return(SYSERR);
    }
    pdev = &devtab[descrp];
    return((pdev->dvinit)(pdev));
}
