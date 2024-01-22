/* malloc.c - malloc(), free() */

/* These routines use xgetheap and xfreeheap to malloc and free */

#include <sys/xinusys.h>

extern char *getmem();

typedef	long		WORD;		/* maximum of (int, char *)	*/

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
	unsigned int	mlen;
	struct mblock	*mnext;
	};

 /*---------------------------------------------------------------------------
  * malloc - call getheap to allocate memory
  *---------------------------------------------------------------------------
  */
 char *malloc(nbytes)
 int nbytes;
 {
	 char	*sb;
	 struct mblock *mbptr;
	 unsigned int dlen;


	 dlen = (unsigned int) roundew(nbytes);
	 nbytes = dlen + sizeof(unsigned int);
	 mbptr = (struct mblock *) getmem(nbytes);
	 if (mbptr != (struct mblock *)NULL) {
		 mbptr->mlen = dlen;
		 sb = (char *) mbptr;
		 sb += sizeof(unsigned int);
		 return(sb);
	 }
	 else
	     return(NULL);
 }


 /*---------------------------------------------------------------------------
  * free - call freeheap to release storage space
  *---------------------------------------------------------------------------
  */
 free(block)
 char *block;
 {
	 struct mblock *mbptr;

	 block -= sizeof(unsigned int);
	 mbptr = (struct mblock *) block;
	 return(freemem((char *)mbptr, mbptr->mlen + sizeof(unsigned int)));
 }


 /*---------------------------------------------------------------------------
  * realloc - realloc the memeory space
  *---------------------------------------------------------------------------
  */
 char *realloc(old, size)
      char *old;
      unsigned int size;
 {
	 unsigned int nbytes;
	 char *new;
	 unsigned int findblklen();

	 nbytes = findblklen(old);

	 if ( size <= nbytes ) {
		 return(old);
	 }
	 if ( (new = malloc(size)) == NULL ) {
		 return(NULL);
	 }
	 bcopy(old, new, nbytes);
	 free(old);
	return(new);
}




/*---------------------------------------------------------------------------
 * findblklen - get the size of the memory actually allocated
 *---------------------------------------------------------------------------
 */
unsigned int findblklen(str)
     char *str;
{
	struct	mblock	*mbtr;

	mbtr = (struct mblock *) (str - sizeof(unsigned int));
	return(mbtr->mlen);
}



struct mblock memlist;
int initialize_getmem = 1;

/*------------------------------------------------------------------------
 * getmem  --  allocate heap storage, returning lowest WORD address
 *------------------------------------------------------------------------
 */
char *getmem(nbytes)
	unsigned nbytes;
{
	struct	mblock	*p, *q, *leftover;

	if (initialize_getmem) {
		initialize_getmem = 0;
		initialize_heap_mem();
	}

	if (nbytes==0 || memlist.mnext== (struct mblock *) NULL) {
		return((char *)NULL);
	}
	nbytes = (unsigned int) roundmb(nbytes);
	for (q= &memlist,p=memlist.mnext ;
	     p != (struct mblock *) NULL ;
	     q=p,p=p->mnext)
		if ( p->mlen == nbytes) {
			q->mnext = p->mnext;
			return( (char *)p );
		} else if ( p->mlen > nbytes ) {
			leftover = (struct mblock *)( (unsigned)p + nbytes );
			q->mnext = leftover;
			leftover->mnext = p->mnext;
			leftover->mlen = p->mlen - nbytes;
			return( (char *)p );
		}
	/* we are out of memory! */
	printf("C library getmem: Ran out of memory!\n");
	return( (char *)NULL );
}


/*------------------------------------------------------------------------
 *  freemem  --  free a memory block, returning it to memlist
 *------------------------------------------------------------------------
 */
freemem(block, size)
	struct	mblock	*block;
	unsigned size;
{
	struct	mblock	*p, *q;
	unsigned top;

	if (size==0) {
		return(0);
	}
	size = (unsigned)roundmb(size);
	for( p=memlist.mnext,q= &memlist;
	    p != (struct mblock *) NULL && p < block ;
	    q=p,p=p->mnext )
	    ;
	if ((top=q->mlen+(unsigned)q)>(unsigned)block && q!= &memlist ||
	    p!=(struct mblock *)NULL &&
	    (size+(unsigned)block) > (unsigned)p ) {
		return(0);
	}
	if ( q!= &memlist && top == (unsigned)block )
	    q->mlen += size;
	else {
		block->mlen = size;
		block->mnext = p;
		q->mnext = block;
		q = block;
	}
	if ( (unsigned)( q->mlen + (unsigned)q ) == (unsigned)p) {
		q->mlen += p->mlen;
		q->mnext = p->mnext;
	}
	return(1);
}


initialize_heap_mem()
{
	char *ptr;
	int numpages;

	numpages = 4096;
	while ( (ptr=(char *)xgetheap(getpagesize()*numpages))
	       == (char *)SYSERR)
	    numpages -= 512;

	memlist.mnext = (struct mblock *) ptr;
	
	memlist.mnext->mnext = (struct mblock *) NULL;
	memlist.mnext->mlen = getpagesize()*numpages;
}

