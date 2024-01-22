/* sprintf.c - sprintf */

#include <varargs.h>

/*------------------------------------------------------------------------
 *  sprintf  --  format arguments and place output in a string
 *------------------------------------------------------------------------
 */
/*VARARGS2*/
sprintf(str, fmt, va_alist)
     char *str;
     char *fmt;
     va_dcl
{
        int     sprntf();
        char    *s;
	va_list	ap;

	va_start(ap);
	
        s = str;

        _doprnt(fmt, ap, sprntf, &s);
        *s++ = '\0';

	va_end(ap);

        return((int)str);
}

/*------------------------------------------------------------------------
 *  sprntf  --  routine called by doprnt to handle each character
 *------------------------------------------------------------------------
 */

static	sprntf(cpp, c)
        char    **cpp;
        int	c;
{
        return(*(*cpp)++ = (char)c);
}


