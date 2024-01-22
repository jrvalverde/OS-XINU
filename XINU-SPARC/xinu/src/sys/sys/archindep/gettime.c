/* gettime.c - gettime, getutim, getrealtim */

#include <conf.h>
#include <kernel.h>
#include <date.h>
#include <sleep.h>
#include <clock.h>

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
SYSCALL	gettime(timvar)
long	*timvar;
{
	u_long	now;

	if (getutim(&now) == SYSERR)
		return(SYSERR);
	*timvar = ut2ltim(now);			/* adjust for timezone	*/
	return(OK);
}

/*------------------------------------------------------------------------
 * getutim  --  obtain time in seconds past Jan 1, 1970, ut (gmt)
 *              (for backward compatibility)
 *------------------------------------------------------------------------
 */
SYSCALL	getutim(timvar)
     u_long	*timvar;
{
    *timvar = clktime;
    return OK;
}

/*------------------------------------------------------------------------
 * getrealtim  --  obtain time since boot 
 *------------------------------------------------------------------------
 */
SYSCALL getrealtim(timvar)
     u_long    *timvar;
{
    u_long ret;
    
    ret = realtime;
    
    if (timvar)
	*timvar = ret;
    return ret;
}
