/* qmapinit.c -- qmapinit */

#include <conf.h>
#include <kernel.h>
#include <qbus.h>
#include <mem.h>
#include <mach.h>

/*------------------------------------------------------------------------
 *  qmapinit -- initialize Microvax II qbus mapping registers
 *		warning: only 4 megs of memory are mapped for dma access
 *------------------------------------------------------------------------
 */

qmapinit()
{
	unsigned int	*pqmapreg;
       	unsigned int	i;

	if (! isUVAXII)
		return(SYSERR);
	*QMMECSR = 1;		/* enable parity error detection */
	for (pqmapreg = QMAP, i = 0; i < QMNMREG; )
		*pqmapreg++ = QMREGV | i++;
	*QMIPCR = QMENAB;	/* enable bus mapping	*/
	if ((int)maxaddr+sizeof(int)-1 > QMNMREG*PAGESIZE)
		kprintf("warning: dma only mapped for 1st %u bytes\n",
			QMNMREG*PAGESIZE);
	return(OK);
}
