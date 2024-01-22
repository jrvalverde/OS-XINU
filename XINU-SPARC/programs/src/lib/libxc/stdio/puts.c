/* puts.c - puts */

#define DEVCONSOLE	readdtable(1)	/* unix output file descriptor	*/

/*------------------------------------------------------------------------
 *  puts  --  write a null-terminated string to the console
 *------------------------------------------------------------------------
 */
puts(s)
register char *s;
{
	register c;
	int	xputc();

	while (c = *s++)
                xputc(DEVCONSOLE,c);
        return(xputc(DEVCONSOLE,'\n'));
}
