/* dvlookup.c - dvlookup */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  dvlookup - return the device number associated with a name string
 *------------------------------------------------------------------------
 */
dvlookup(name)
char *name;
{
    int i;

    for (i = 0; i < NDEVS; ++i) {
	    if (strcmp(name,devtab[i].dvname) == 0)
		return(i);
    }

    return(SYSERR);
}
