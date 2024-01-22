/* fingerd.c - fingerd */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <nettcp.h>

/*#define	FINGERLOG	CONSOLE*/

/*------------------------------------------------------------------------
 *  fingerd  -  the finger server
 *------------------------------------------------------------------------
 */
PROCESS fingerd()
{
    int	fd;
    
    fd = open(TCP, ANYFPORT, 79);	/* a stub LISTEN state TCP fd	*/
    control(fd, TCPC_LISTENQ, 5);	/* set the queue length		*/
    
    while (TRUE) {
	char	user[128];
	int	cfd, cc;
	
	/* cfd is a CONNECTED TCP stream when control() returns */
	cfd = control(fd, TCPC_ACCEPT);
	
	if (cfd == SYSERR) {
	    break;
	}
	
	/* just handle it directly */
	
	cc = read(cfd, user, sizeof(user));
	if (cc < 2) {
	    close(cfd);
	    continue;
	}
	user[cc-2] = '\0';	/* format is "[user]\r\n"	*/
	
#ifdef FINGERLOG
	{
	    struct	tcpstat tcpstat;
	    IPaddr	from;
	    char	str[80];
	    
	    control(cfd, TCPC_STATUS, &tcpstat);
	    IP_COPYADDR(from, tcpstat.ts_faddr);
	    sprintf(str, "fingerd: %-32s from %u.%u.%u.%u\n", user,
		    BYTE(from, 0), BYTE(from, 1),
		    BYTE(from, 2), BYTE(from, 3));
	    write(FINGERLOG, str, strlen(str));
	}
#endif
	
	lfing(user, cfd);
	close(cfd);
    }
}
