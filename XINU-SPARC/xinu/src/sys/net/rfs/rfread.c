/* rfread.c - rfread */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  rfread  --  read one or more bytes from a remote file
 *------------------------------------------------------------------------
 */
rfread(devptr, buff, len)
struct	devsw	*devptr;
char	*buff;
int	len;
{
	int cc;
	int reqlen;
	int readlen;

	readlen = 0;
	while (len > 0) {
		reqlen = min(len,RDATLEN);
		cc = rfio(devptr, FS_READ, buff, reqlen);
		if (cc <= 0) {
			if (readlen == 0)
			    /* error right away */
			    return(cc);
			else
			    /* catch the error next time */
			    return(readlen);
		}
		buff += cc;
		readlen += cc;
		len -= cc;
	}
	return(readlen);
}
