/* 
 * x_rtt.c - rtt test
 * 
 * Author:	
 * 		Dept. of Computer Science
 * 		Purdue University
 * Date:	Mon Aug  2 16:14:26 1993
 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <network.h>
#include <nettcp.h>

#define MAX_SIZE 	4096
/*------------------------------------------------------------------------
 *  x_rtt - rtt testing
 *------------------------------------------------------------------------
 */
COMMAND	x_rtt(stdin, stdout, stderr, nargs, argv)
int	stdin, stdout, stderr, nargs;
char	*argv[];
{
    int size = 512;			/* packet size */
    int npackets= 20;			/* # of packets */
    char str[80], buf[MAX_SIZE];
    int fd, n = 0, cc, count;
    IPaddr	addr;
    char host[22];		/* allows "XXX.XXX.XXX.XXX:XXXXX\0" */

    switch (nargs) {
      case 4:
	npackets = atoi(argv[4]);
	/* fall through */
      case 3:
	size = atoi(argv[2]);
	if (size > MAX_SIZE)
	    size = MAX_SIZE;
	break;
      case 2:
	break;
      default:
	sprintf(str, "usage: rtt host [size] [# packets]\n");
	write(stderr, str, strlen(str));
	return OK;
    }

    if (name2ip(addr, argv[1]) == SYSERR) {
	write(stdout, "unknown host\n", 13);
	return SYSERR;
    }
    /* echo port */
    sprintf(host, "%u.%u.%u.%u:7", BYTE(addr, 0), BYTE(addr, 1),
	    BYTE(addr, 2), BYTE(addr, 3));

    fd = open(TCP, host, ANYLPORT);
    if (fd < 0) {
	sprintf(buf, "open: %s\n", tcperror[-fd]);
	write(stderr, str, strlen(str));
	return SYSERR;
    }
    
    while (n < npackets) {
	write(fd, buf, size);
	n++;
	count = size;
	while ((cc = read(fd, buf, count)) > 0) {
	    if (cc == SYSERR) {
		write(stderr, "read returned SYSERR\n", 21);
		break;
	    }
	    count -= cc;
	    if (count == 0)
		break;
	}
	if (cc < 0) {
	    sprintf(str, "read: %s\n", tcperror[-cc]);
	    write(stderr, str, strlen(str));
	}
    }
    close(fd);
    return OK;
}


