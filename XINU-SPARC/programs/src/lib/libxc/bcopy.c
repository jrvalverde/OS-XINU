/* bcopy.c - bcopy */

#include <sys/xinusys.h>

/*------------------------------------------------------------------------
 *  bcopy  -  copy a block of memory form one location to another
 *------------------------------------------------------------------------
 */
bcopy(from, to, nbytes)
register char	*from;
register char	*to;
register int	nbytes;
{
	while (--nbytes >= 0)
		*to++ = *from++;
	return(OK);
}

/*------------------------------------------------------------------------
 *  fbcopy  -  fast copy a block of memory form one location to another.
 		 NOTE that TO & FROM ADDRESSES MUST BE INT ADDRESSES !
		 There is a macro "isintaddr()" in mem.h for you to use.
 *------------------------------------------------------------------------
 */
fbcopy(from, to, nbytes)
register int	*from;
register int	*to;
int	nbytes;
{
	register int nints;
	short	 bytes;

	nints = nbytes/sizeof(int);
	bytes = nbytes%sizeof(int);
	
	while (--nints >= 0)
		*to++ = *from++;
	if (bytes != 0) {
	    while (--bytes >= 0)
		*(char *)to++ = *(char *)from++;
        }
	return(OK);
}
