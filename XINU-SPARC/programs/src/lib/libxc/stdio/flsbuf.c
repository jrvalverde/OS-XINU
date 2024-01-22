/* flsbuf.c - flsbuf(), fflush(), fclose() */

#include <stdio.h>

fclose(ioptr)
FILE *ioptr;
{
	int retval;

	retval = EOF;
	if(ioptr == NULL)
		return(retval);

	ioptr->_flag = 0;
	ioptr->_cnt = 0;
	ioptr->_ptr = ioptr->_base = NULL;
	ioptr->_bufsiz = 0;

	if(close(fileno(ioptr)) < 0) {
		ioptr->_file = -1;
		return(EOF);		/* error */
	}
	else {
		ioptr->_file = -1;
		return(0);		/* success */
	}
}


fflush(ioptr)
FILE *ioptr;
{
	return(0);			/* don't have to do anything */
}

_flsbuf(c, ioptr)
char c;
FILE *ioptr;
{
	char buf[32];

	buf[0] = c;
	if (write(fileno(ioptr), buf, 1) == 1)
	    return(c);
	else {
		ioptr->_flag |= _IOERR;
		return(EOF);
	}
}

