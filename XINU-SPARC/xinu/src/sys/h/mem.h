/* mem.h - freestk, roundew, truncew */

/*----------------------------------------------------------------------
 * roundew, truncew - round or trunctate address to next even word
 *----------------------------------------------------------------------
 */
#define	roundew(x)	(WORD *)( (3 + (WORD)(x)) & ~03 )
#define	truncew(x)	(WORD *)( ((WORD)(x)) & ~03 )
#define isintaddr(x)	!((unsigned)x % sizeof(int))


/*----------------------------------------------------------------------
 * roundmb, truncmb -- round or truncate address up to size of mblock
 *----------------------------------------------------------------------
 */
#define	roundmb(x)	(WORD *)( (7 + (WORD)(x)) & ~07 )
#define	truncmb(x)	(WORD *)( ((WORD)(x)) & ~07 )


struct	mblock	{
	struct	mblock	*mnext;
	unsigned int	mlen;
};
extern	struct	mblock	memlist;	/* head of free memory list	*/
extern	WORD	*maxaddr;		/* max memory address		*/
extern	WORD	_end;			/* address beyond loaded memory	*/
extern	WORD	*end;			/* &_end + FILLSIZE		*/

/* Stack Frame Layout - known offsets */
#define	IREG_0_OFFSET		32	/* %i0 offset 			*/
#define	LREG_0_OFFSET		0	/* %l0 offset 			*/
#define HIDE_VAL_OFFSET		64	/* hidden parameter offset	*/
#define SAV_IREG_0_OFFSET	68	/* saved %i0 value's offset	*/
#define SAV_IREG_6_OFFSET	92	/* saved %i6 value's offset	*/
