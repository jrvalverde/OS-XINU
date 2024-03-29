/* othinit.c - othinit */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <q.h>

#if	Noth > 0

/* NOTE: This MUST be called AFTER ethinit for the simulating device */

static	noth = 0;	/* number of othernets initialized	*/

extern	struct	otblk	oth[];
struct  etblk   eth[Neth];

/*------------------------------------------------------------------------
 *  othinit  -  initialize Othernet
 *------------------------------------------------------------------------
 */
int othinit(devptr)
struct	devsw	*devptr;
{
	struct	otblk	*otptr;
	struct	etdev	*etptr;
	int	dev;
	int	i;

#ifdef DEBUG
	kprintf("othinit(dev) called\n");
#endif	
	++noth;
	if (noth > Noth)
		panic("othinit: not enough otblk space\n");
	otptr = &oth[noth-1];
	devptr->dvioblk = (char *)otptr;

	/* set the physical device */

	dev = ETHER;	/* hardwired, for now */

	otptr->ot_pdev = dev;
	otptr->ot_intf = -1;
	otptr->ot_descr = "FICTIONAL OTHERWARE, rev 0.1";

	etptr = (struct etdev *) devtab[dev].dvioblk;
	if (etptr == 0)
		panic("othinit before ethinit\n");

	/* start with the Ether addr */
	blkcopy(otptr->ot_paddr, etptr->ed_paddr, EP_ALEN);

	/* and perturb it a little... */

	otptr->ot_paddr[0] |= 1;	/* multicast bit */
	otptr->ot_paddr[3] = noth;
	otptr->ot_paddr[4] = BINGID;

	/* broadcast addr is physaddr with byte 5 0xff */

	blkcopy(otptr->ot_baddr, otptr->ot_paddr, EP_ALEN);
	otptr->ot_baddr[5] = 0xff;

	otptr->ot_valid = TRUE;
	/* drop it in the etptr array */

	for (i=0; i<Noth; ++i)
		if (eth[0].etoth[i] == 0 
		|| !eth[0].etoth[i]->ot_valid)
			break;
	if (i == Noth)
		panic("othinit: no space in etoth[]\n");
	eth[0].etoth[i] = otptr;

	/* tell the physical device */
	control(dev, EPC_MADD, otptr->ot_paddr);
	control(dev, EPC_MADD, otptr->ot_baddr);
	return OK;

}

struct	otblk	oth[Noth];
#endif	/* Noth > 0 */
