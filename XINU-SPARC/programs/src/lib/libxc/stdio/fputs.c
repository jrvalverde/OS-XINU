/* fputs.c - fputs */

#include <stdio.h>

/*------------------------------------------------------------------------
 *  fputs  --  write a null-terminated string to a device (file)
 *------------------------------------------------------------------------
 */
fputs(s, stream)
char *s;
FILE *stream;
{
	int r;
	int c;
	int dev;
	int	xputc();

	dev = readdtable(stream->_file);
	while (c = *s++)
                r = xputc(dev, c);
	return(r);
}
