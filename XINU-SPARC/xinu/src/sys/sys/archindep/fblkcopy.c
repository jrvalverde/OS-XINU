/* fblkcopy.c - fblkcopy */

/* I don't think this routine is needed any more, since I have optimized */
/* blkcopy now.   Griff 5-12-92 */

#include <kernel.h>

/*#define USE_FAST_BLKCOPY*/


#ifdef USE_FAST_BLKCOPY
/* change these for your machine */
#define CUTOFF		100
#define INTSIZE 4
/* do not change these without changing the code */
#define I_UNROLLSIZE	10
#define C_UNROLLSIZE	10
#endif

/*------------------------------------------------------------------------
 *  fblkcopy  -  copy a block of memory from one location to another
 *------------------------------------------------------------------------
 */

#ifndef USE_FAST_BLKCOPY

fblkcopy(to, from, nbytes)
register char	*to;
register char	*from;
register int	nbytes;
{
	while (--nbytes >= 0)
		*to++ = *from++;
	return(OK);
}

#endif



#ifdef USE_FAST_BLKCOPY

fblkcopy(to, from, nbytes)
register char	*to;
register char	*from;
register int	nbytes;
{
	if (nbytes > CUTOFF) {
#ifdef SUN3
		/* The sun will let you copy int when non-aligned */
		if (1) {
			register int *t;
			register int *f;

			t = (int *) to;
			f = (int *) from;
			nbytes -= (I_UNROLLSIZE*INTSIZE);
			while (nbytes > 0) {
				*(t) = *(f);
				*(t+1) = *(f+1);
				*(t+2) = *(f+2);
				*(t+3) = *(f+3);
				*(t+4) = *(f+4);
				*(t+5) = *(f+5);
				*(t+6) = *(f+6);
				*(t+7) = *(f+7);
				*(t+8) = *(f+8);
				*(t+9) = *(f+9);
				t += I_UNROLLSIZE;
				f += I_UNROLLSIZE;
				nbytes -= (I_UNROLLSIZE*INTSIZE);
			}
			nbytes += (I_UNROLLSIZE*INTSIZE);

			to = (char *) t;
			from = (char *) f;
		}
#else
		if (((int) to & 3) == ((int) from & 3)) {
			/* same word offset, copy in big pieces */
			register int *t;
			register int *f;

			/* in case 'to' and 'from' are NOT on word boundary */
			*(to+0) = *(from+0);
			*(to+1) = *(from+1);
			*(to+2) = *(from+2);
			t = (int *) (((unsigned)to+3) & ~3);
			f = (int *) (((unsigned)from+3) & ~3);
			nbytes += ((int) t - (int) to);
			nbytes -= (I_UNROLLSIZE*INTSIZE);
			while (nbytes > 0) {
				*(t) = *(f);
				*(t+1) = *(f+1);
				*(t+2) = *(f+2);
				*(t+3) = *(f+3);
				*(t+4) = *(f+4);
				*(t+5) = *(f+5);
				*(t+6) = *(f+6);
				*(t+7) = *(f+7);
				*(t+8) = *(f+8);
				*(t+9) = *(f+9);
				t += I_UNROLLSIZE;
				f += I_UNROLLSIZE;
				nbytes -= (I_UNROLLSIZE*INTSIZE);
			}
			nbytes += (I_UNROLLSIZE*INTSIZE);

			to = (char *) t;
			from = (char *) f;
		}
#endif
		else {
			/* different word offset, copy as chars */
			nbytes -= C_UNROLLSIZE;
			while (nbytes > 0) {
				*(to) = *(from);
				*(to+1) = *(from+1);
				*(to+2) = *(from+2);
				*(to+3) = *(from+3);
				*(to+4) = *(from+4);
				*(to+5) = *(from+5);
				*(to+6) = *(from+6);
				*(to+7) = *(from+7);
				*(to+8) = *(from+8);
				*(to+9) = *(from+9);
				to += C_UNROLLSIZE;
				from += C_UNROLLSIZE;
				nbytes -= C_UNROLLSIZE;
			}
			nbytes += C_UNROLLSIZE;
		}
	}

	/* finish any leftover piece */
	while (--nbytes >= 0)
	    *to++ = *from++;
	return(OK);
}
#endif

