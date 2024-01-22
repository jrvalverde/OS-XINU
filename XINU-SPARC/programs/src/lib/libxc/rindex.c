/*
 * Return the ptr in sp at which the character c last
 * appears; NULL if not found
*/

#define NULL 0

char *
rindex(sp, c)
register char *sp;
register c;
{
	register char *r;

	r = NULL;
	do {
		if (*sp == (char)c)
			r = sp;
	} while (*sp++);
	return(r);
}
