/* scanf.c - scanf, fscanf, sscanf, getch, ungetch, sgetch, sungetch */

typedef int	Bool;

#include <conf.h>
#include <io.h>
#include <tty.h>
#include <varargs.h>

#define	CONSOLE	0
#define	EOF	(-1)
#define	EMPTY	(-2)


/*------------------------------------------------------------------------
 *  scanf  --  read from the console according to a format
 *------------------------------------------------------------------------
 */
/*VARARGS1*/
scanf(fmt, va_alist)
     char	*fmt;
     va_dcl
{
	int	getch();
	int	ungetch();
	int	buf;
	int	ret;
	va_list	ap;

	va_start(ap);

	buf = EMPTY;
	ret = _doscan(fmt, ap, getch, ungetch, CONSOLE, &buf);

	va_end(ap);
	return(ret);
}

/*------------------------------------------------------------------------
 *  fscanf  --  read from a device (file) according to a format
 *------------------------------------------------------------------------
 */
fscanf(dev, fmt, va_alist)
     int	dev;
     char	*fmt;
     va_dcl
{
	int	getch();
	int	ungetch();
	int	buf;
	int	ret;
	va_list	ap;

	va_start(ap);

	buf = EMPTY;
	ret = _doscan(fmt, ap, getch, ungetch, dev, &buf);

	va_end(ap);
	return(ret);
}

/*------------------------------------------------------------------------
 *  getch  --  get a character from a device with pushback
 *------------------------------------------------------------------------
 */
static	getch(dev, buf)
	int	dev;
	int	*buf;
{
	if( *buf != EOF && *buf != EMPTY)
		*buf = getc(dev);
	if( *buf != EOF )
		*buf = control(dev, TCNEXTC);
	return(*buf);
}

/*------------------------------------------------------------------------
 *  ungetch  --  pushback a character for getch
 *------------------------------------------------------------------------
 */
static	ungetch(dev, buf)
	int	dev;
	int	*buf;
{
	*buf = EMPTY;
}

/*------------------------------------------------------------------------
 *  sscanf  --  read from a string according to a format
 *------------------------------------------------------------------------
 */
sscanf(str, fmt, va_alist)
     char	*str;
     char	*fmt;
     va_dcl
{
	int	sgetch();
	int	sungetch();
	char	*s;
	int	ret;
	va_list	ap;

	va_start(ap);

	s = str;
	ret = _doscan(fmt, ap, sgetch, sungetch, 0, &s);

	va_end(ap);
	return(ret);
}

/*------------------------------------------------------------------------
 *  sgetch  -- get the next character from a string
 *------------------------------------------------------------------------
 */
static	sgetch(dummy,cpp)
	int	dummy;
	char	**cpp;
{
	return( *(*cpp) == '\0' ? EOF : *(*cpp)++ );
}

/*------------------------------------------------------------------------
 *  sungetc  --  pushback a character in a string
 *------------------------------------------------------------------------
 */
static	sungetch(dummy,cpp)
	int	dummy;
	char	**cpp;
{
	return(*(*cpp)--);
}
