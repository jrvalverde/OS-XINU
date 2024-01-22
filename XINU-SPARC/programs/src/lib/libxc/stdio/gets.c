/* gets.c - gets */

#include <sys/xinusys.h>

#define DEVCONSOLE	readdtable(0)

#define	TTY_EOF	'\004'	/* control-d is end-of-file		*/

/*------------------------------------------------------------------------
 *  gets  -- gets string from the console device reading to user buffer
 *------------------------------------------------------------------------
 */
char *gets(s)
        char *s;
{
	register c;
	register char *cs;

	cs = s;
        while ((c = xgetc(DEVCONSOLE)) != '\n' && c != '\r' && c != TTY_EOF)
		*cs++ = c;
	if (c==TTY_EOF && cs==s)
		return(NULL);
	*cs++ = '\0';
	return(s);
}
