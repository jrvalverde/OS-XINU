/* 
 * udpdiscard.c - UDP server for "discard" service
 * 
 * 		Dept. of Computer Science
 * 		Purdue University
 * Date:	Fri Apr  2 13:34:45 1993
 */

/*#define DEBUG*/

#include <conf.h>
#include <kernel.h>
#include <network.h>

#define	MAXPACK	1600		/* maximum size of received datagram	*/
static	char	buff[MAXPACK];	/* here because the stack may be small	*/

/*------------------------------------------------------------------------
 *  udpdiscard  -  UDP discard server process (runs forever in background)
 *------------------------------------------------------------------------
 */
PROCESS udpdiscard()
{
    int	dev, len;
    
    if ( (dev=open(UDP, ANYFPORT, UP_DISCARD)) == SYSERR) {
	printf("udpdiscard: open fails\n");
	return(SYSERR);
    }
    while ( TRUE ) {
	len = read(dev, buff, MAXPACK);
#ifdef DEBUG
	kprintf("udpdiscard: len = %d\n", len);
#endif
    }
}
