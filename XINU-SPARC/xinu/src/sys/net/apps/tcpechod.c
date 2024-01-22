/* tcpechod.c - tcpechod */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <nettcp.h>

/*#define DEBUG*/
#define PRINTERRORS


/*------------------------------------------------------------------------
 *  tcpechod  -  the TCP echo server
 *------------------------------------------------------------------------
 */
PROCESS	tcpechod()
{
    int	fd;
    
    fd = open(TCP, ANYFPORT, 7);	/* a stub LISTEN state TCP fd */
    control(fd, TCPC_LISTENQ, 10);	/* set the queue length */
    
    while (TRUE) {
	int	cfd;
	
	/* cfd is a CONNECTED TCP stream when control() returns */
	if ((cfd = control(fd, TCPC_ACCEPT)) == SYSERR) {
	    break;
	}
	
	resume(kcreate(TCPECHOP, TCPECHOSTK, TCPECHOPRI, TCPECHOPNAM, 1, cfd));
	/* closed by child */
    }
    
}

/*------------------------------------------------------------------------
 *  tcpechop  -  handle a particular TCP echo request
 *------------------------------------------------------------------------
 */
PROCESS tcpechop(cfd)
     int	cfd;
{
    int	cc, wr;
    char	buf[1024];
    int	nread;
    int	nwrite;
#ifdef DEBUG
    kprintf("tcpechod: accepted connection on dev %d\n", cfd);
#endif
    
    nread = nwrite = 0;
    while (TRUE) {
	cc = read(cfd, buf, sizeof(buf));
	if (cc == TCPE_URGENTMODE || cc == TCPE_NORMALMODE)
	    continue;
	if (cc <= 0) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    if (cc != 0)
		kprintf("tcpechod: read error: %d\n", cc);
#endif
	    break;
	}
	nread += cc;
	
	wr = write(cfd, buf, cc);
	if (wr <= 0) {
#if defined(DEBUG) || defined(PRINTERRORS)
	    kprintf("tcpechod: write error: %d\n", wr);
#endif
	    break;
	}
	nwrite += wr;
    }
    close(cfd);
#ifdef DEBUG
    kprintf("tcpechod: read %d chars, wrote %d\n", nread, nwrite);
#endif
}
