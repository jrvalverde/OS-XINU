/* sysopen.c - sysopen */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  sysopen  -  syscal interface to the kernel open call, first argument
 *              is the string name of the device, rather the the device
 *              number
 *------------------------------------------------------------------------
 */
sysopen(nm_descrp, nam, mode)
char    *nm_descrp;
char	*nam;
char	*mode;
{
	if (nm_descrp == NULL)
	    return(SYSERR);
	
	return(open(dvlookup(nm_descrp), nam, mode));
}
