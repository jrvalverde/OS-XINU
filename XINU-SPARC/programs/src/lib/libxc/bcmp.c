/* bcmp.c - bcmp */

/*------------------------------------------------------------------------
 *  bcmp  -  return 0 iff one block of memory is equal to another
 *------------------------------------------------------------------------
 */
bcmp(first, second, nbytes)
register char	*first;
register char	*second;
register int	nbytes;
{
	while (--nbytes >= 0)
		if (*first++ != *second++)
			return(-1);
	return(0);
}
