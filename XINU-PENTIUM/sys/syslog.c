/* syslog.c - syslog */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <io.h>
#include <syslog.h>
#include <bootp.h>

static int	logdev = BADDEV;
static char	logbuf[LOG_MAXLEN+10]; /* mesg. + room for <#.> & null	*/

extern int	currpid;

/*------------------------------------------------------------------------
 * syslog - log messages to a remote UNIX syslogd or the console
 *------------------------------------------------------------------------
 */
int
syslog(where, pname, fmt, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15)
int	where;
char	*pname, *fmt;
int	a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15;
{
    int	len;
#ifndef LOGHOST
    return kprintf(fmt, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,					a12,a13,a14,a15);
#else
    if (logdev == BADDEV) {
	logdev = open(UDP, LOGHOST, ANYLPORT);
	if (logdev == SYSERR)
	    return SYSERR;
	control(logdev, DG_SETMODE, DG_DMODE);
    }
    sprintf(logbuf, "<%d>", where);
    len = strlen(logbuf);
    sprintf(&logbuf[len], "%s[%u]: ", pname, currpid);
    len = strlen(logbuf);
    sprintf(&logbuf[len], fmt, a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,
	    a12,a13,a14,a15);
    len = strlen(logbuf);
    logbuf[len++] = '\n';
    logbuf[len] = '\0';
    write(logdev, logbuf, len);
#endif
}
