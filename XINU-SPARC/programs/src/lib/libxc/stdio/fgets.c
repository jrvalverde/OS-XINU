/* fgets.c - fgets */

#include <stdio.h>

/*------------------------------------------------------------------------
 *  fgets  --  read a newline-terminated string from device (file) dev
 *------------------------------------------------------------------------
 */
char *fgets(s, n, fp)
char    *s;
int      n;
FILE    *fp;
{
	register c;
	register char *cs;
	register int dev;

	dev = readdtable(fileno(fp));
	cs = s;
        while (--n>0 && (c = xgetc(dev))>=0) {
		*cs++ = c;
		if (c=='\n')
			break;
	}
	if (c<0 && cs==s)
		return(NULL);
	*cs++ = '\0';
	return(s);
}
