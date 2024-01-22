/* procreg.c -- mtpr, mfpr */

/*
 *------------------------------------------------------------------------
 * mtpr -- move int to priveleged processor register
 *------------------------------------------------------------------------
 */

mtpr(val, reg)
int val, reg;
{
	asm ("mtpr 4(ap), 8(ap)");	/* move val to register reg */
}

/*
 *-----------------------------------------------------------------------
 * mfpr -- return int from privileged processor register
 *-----------------------------------------------------------------------
*/

mfpr(reg)
int reg;
{
	asm ("mfpr 4(ap), r0");
}
