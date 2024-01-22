/* ioinit.c - ioinit, iosetvec */

#include <conf.h>
#include <kernel.h>
#include <iodisp.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  ioinit --  standard interrupt vector and dispatch initialization
 *------------------------------------------------------------------------
 */
ioinit(descrp)
int	descrp;
{
	int	minor;

	if (isbaddev(descrp) )
		return(SYSERR);
	minor = devtab[descrp].dvminor;
	iosetvec(descrp, minor, minor);
	return(OK);
}

/*------------------------------------------------------------------------
 *  iosetvec  -  fill in interrupt vectors and dispatch table entries
 *------------------------------------------------------------------------
 */
iosetvec(descrp, incode, outcode)
int	descrp;
int	incode;
int	outcode;
{
	struct	devsw	*devptr;
	struct	intmap	*map;
	int	*vptr;

	if (isbaddev(descrp))
		return(SYSERR);
	devptr = &devtab[descrp];
	map = &intmap[devptr->dvnum];	/* fill in interrupt dispatch	*/
	map->iin =  devptr->dviint;	/*   map with addresses of high-*/
	map->icode = incode;		/*   level input and output	*/
	map->iout = devptr->dvoint;	/*   interrupt handlers and	*/
	map->ocode = outcode;		/*   minor device numbers	*/
	vptr = (int *)devptr->dvivec;
	if (vptr != (int *)NULL)
		*vptr = INTVECI+(descrp*2*DVECSIZE);/* fill in input interrupt*/
	vptr = (int *)devptr->dvovec;
	if (vptr != (int *)NULL)
		*vptr = INTVECO+(descrp*2*DVECSIZE);/* fill in outpt interrupt*/
	return(OK);
}
