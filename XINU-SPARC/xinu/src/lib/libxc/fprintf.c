/* fprintf.c - fprintf */

#include <varargs.h>
#include <kernel.h>

#define BUFSIZE 256
static fputc();
static fflush();
static struct buffer {
	int		dev;
	char		buf[BUFSIZE];
	unsigned	len;
};


/*------------------------------------------------------------------------
 *  fprintf  --  print a formatted message on specified device (file)
 *               buffered version -- sdo -- Wed Jul  3, 1991
 *------------------------------------------------------------------------
 */
fprintf(dev, fmt, va_alist)
     int  dev;
     char *fmt;
     va_dcl
{
	struct buffer buffer;
	va_list	ap;

	va_start(ap);

	buffer.len = 0;
	buffer.dev = dev;

        _doprnt(fmt, ap, fputc, &buffer);

	va_end(ap);

	return(fflush(&buffer));
}


static
fputc(pbuf,ch)
     struct buffer *pbuf;
     int	ch;
{
	pbuf->buf[pbuf->len++] = (char)ch;
	if (pbuf->len >= BUFSIZE)
	    return(fflush(pbuf));
	return(OK);
}
     

static
fflush(pbuf)
     struct buffer *pbuf;
{
	int ret;
	if (pbuf) {
		ret = write(pbuf->dev,pbuf->buf,pbuf->len);
		pbuf->len = 0;
		if (ret != 1)
		    return(SYSERR);
	}
	return(OK);
}
