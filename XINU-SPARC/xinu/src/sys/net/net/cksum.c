/* cksum.c - cksum */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  cksum  -  Return 16-bit ones complement of 16-bit ones complement sum 
 *------------------------------------------------------------------------
 */
short cksum(psh, nwords)
unsigned short	*psh;
int		nwords;
{
	unsigned long	sum;

	/* unrolled loop */
	sum = 0;
	while (nwords >= 10) {
		sum += psh[0];
		sum += psh[1];
		sum += psh[2];
		sum += psh[3];
		sum += psh[4];
		sum += psh[5];
		sum += psh[6];
		sum += psh[7];
		sum += psh[8];
		sum += psh[9];
		psh += 10;
		nwords -= 10;
	}

	/* slow loop to clean up */
	while (nwords > 0) {
		sum += *psh++;
		--nwords;
	}

	sum = (sum >> 16) + (sum & 0xffff);	/* add in carry   */
	sum += (sum >> 16);			/* maybe one more */
	return ~sum;
}
