/* a1rwint.c - a1readint, a1writeint */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <snmp.h>
#include <asn1.h>

/*------------------------------------------------------------------------
 * a1readint - convert an ASN.1 encoded integer into a machine integer
 *------------------------------------------------------------------------
 */
int a1readint(pack, len)
u_char	*pack;
int	len;
{
	register int	tot;
	u_char 		neg;
	int		tlen;
	
	if ((tlen = len) > sizeof(int))
		return 0;
	tot = *pack & ~CHAR_HIBIT;
	neg = *pack & CHAR_HIBIT;
	for (tlen--, pack++ ; tlen > 0 ; tlen--, pack++)
		tot = (tot << CHAR_BITS) | (int) *pack;
	if (neg)
		tot -= (1 << ((len * CHAR_BITS) - 1));
	return tot;
}

/*------------------------------------------------------------------------
 * a1writeint - convert an integer into its ASN.1 encoded form
 *------------------------------------------------------------------------
 */
int a1writeint(val, buffp, a1type)
int	val;
u_char	*buffp;
int	a1type;
{
	unsigned	tmp, numbytes;
	register u_char	*bp;
	
	bp = buffp;
	tmp = val;
	if (A1_SIGNED(a1type) && val < 0)
		tmp = -val;
	if (tmp < (unsigned) CHAR_HIBIT)
		*bp++ = numbytes = (u_char) 1;
	else if (tmp < (unsigned) BYTE2_HIBIT)
		*bp++ = numbytes = (u_char) 2;
	else if (tmp < (unsigned) BYTE3_HIBIT)
		*bp++ = numbytes = (u_char) 3;
	else if (tmp < (unsigned) BYTE4_HIBIT)
		*bp++ = numbytes = (u_char) 4;
	else {	/* 5 bytes for unsigned with high bit set */
		*bp++ = (u_char) 5;	/* length */
		*bp++ = (u_char) 0;
		numbytes = 4;
	}
	val = hl2net( val );
	blkcopy(bp, ((char *) &val) + (sizeof(int) - numbytes), numbytes);
	bp += numbytes;
	return bp - buffp;
}
