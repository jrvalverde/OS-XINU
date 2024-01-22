/* filbuf.c - filbuf() */

#include <stdio.h>

_filbuf(ioptr)
FILE *ioptr;
{
	char buf[32];
	int cnt;

	cnt = read(fileno(ioptr), buf, 1);
	if (cnt == 1)
	    return(buf[0]);
	else if (cnt == 0) {
		return(EOF);
	}
	else {				/* cnt == -1 */
		ioptr->_flag |= _IOERR;
		return(EOF);
	}
	
}
