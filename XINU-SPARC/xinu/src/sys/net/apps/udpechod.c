/* udpechod.c - udpechod */

#include <conf.h>
#include <kernel.h>
#include <network.h>

#define	MAXECHO	5000		/* maximum size of echoed datagram	*/
static	char	buff[MAXECHO];	/* here because the stack may be small	*/

/*------------------------------------------------------------------------
 *  udpechod  -  UDP echo server process (runs forever in background)
 *------------------------------------------------------------------------
 */
PROCESS	udpechod()
{
	int	dev, len;

	if ( (dev=open(UDP, ANYFPORT, UP_ECHO)) == SYSERR) {
		printf("udpechod: open fails\n");
		return(SYSERR);
	}
	while ( TRUE ) {
		len = read(dev, buff, MAXECHO);
		write(dev, buff, len);
	}
}
