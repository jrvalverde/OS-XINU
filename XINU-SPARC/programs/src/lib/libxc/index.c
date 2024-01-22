/*
 * Return the ptr in sp at which the character c appears;
 * NULL if not found
 */

#define	NULL	0

char *
index(sp, c)
register char *sp;
register c;
{
	do {
		if (*sp == (char)c)
			return(sp);
	} while (*sp++);
	return(NULL);
}
