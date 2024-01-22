/* mem.h - freestk, roundew, truncew */

#define PAGESIZE	512

struct	mblock	{
	struct	mblock	*mnext;
	unsigned int	mlen;
	};

/*----------------------------------------------------------------------
 * roundew, truncew - round or truncate address to next sizeof(mblock)
 *----------------------------------------------------------------------
 */
#define	roundew(x)	(int *)( ((sizeof(struct mblock)-1) + (int)(x))	\
					& (~(sizeof(struct mblock)-1)) )

#define	truncew(x)	(int *)( ((int)(x)) & (~(sizeof(struct mblock)-1)) )

/*----------------------------------------------------------------------
 *  freestk  --  free stack memory allocated by getstk
 *----------------------------------------------------------------------
 */
#define freestk(p,len)	freemem((unsigned)(p)			\
				- (unsigned)(roundew(len))	\
				+ (unsigned)sizeof(int),	\
				roundew(len) )

extern	struct	mblock	memlist;	/* head of free memory list	*/
extern	int	*maxaddr;		/* max memory address		*/
extern	int	end;			/* address beyond loaded memory	*/
extern	int	etext;			/* address beyond text segment	*/
extern	int	edata;			/* address beyond data segment	*/
extern	int	end;			/* address beyond loaded memory	*/
