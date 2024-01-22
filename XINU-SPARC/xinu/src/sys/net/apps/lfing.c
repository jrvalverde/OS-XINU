/* lfing.c - lfing */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  lfing  -  print local FINGER info on the given descriptor
 *------------------------------------------------------------------------
 */
int lfing(nam, output)
     char	*nam;
     int	output;
{
    char *header = "Login       Name\n";
    char *buf    = "xinu        Joe Random Xinu User\n";
    
    if (nam == NULL || *nam == '\0' || *nam == '\r') {
	/* short form */
	/* not really here yet... fake it */
	
	write(output, header, strlen(header));
	write(output, buf, strlen(buf));
	return OK;
    }
    write(output, "Not implemented\n", 16);
    /* not implemented (yet) */
    return SYSERR;
}
