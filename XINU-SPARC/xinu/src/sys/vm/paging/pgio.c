/*----------------------------------------------------------------------
 * pgio.c - Paging sender/receiver process C source file
 *----------------------------------------------------------------------
 */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <vmem.h>
#include <frame.h>
#include <pgio.h>

/* variables used for statistical purposes */
u_long	tmptime;			/* temp variable to store time	*/
u_long	accouttime=0;			/* accumulated page out time	*/
u_long	accintime=0;			/* accumulated page in time	*/
u_long	intimes[ITIMSIZE];		/* array of in times		*/
u_long	outtimes[OTIMSIZE];		/* array of out times		*/
u_long	cintimes = 0;			/* count of pgs read in		*/
u_long	couttimes = 0;			/* count of pgs sent out	*/

/* global paging variables */
int     psready=0;    		/* ==1 after PS accepted start request  */

/* paging statistic routines */

/*---------------------------------------------------------------------------
 * avouttime - calculate the average page out time in microseconds
 *---------------------------------------------------------------------------
 */
u_long avouttime()
{
    if (pg.pagingouts != 0)
	return((u_long)(accouttime/pg.pagingouts));
    else
	return(0);
}

/*---------------------------------------------------------------------------
 * avintime - calculate the average page in time in microseconds
 *---------------------------------------------------------------------------
 */
u_long avintime()
{
    if (pg.diskfaults != 0)
	return((u_long)(accintime/pg.diskfaults));
    else
	return(0);
}


/*-------------------------------------------------------------------------
 * pgiostart - start the page server processes
 *-------------------------------------------------------------------------
 */
pgiostart()
{
#ifdef XPP_PAGING
    xpppgioinit();
    xpppgiostart();
#endif
    
#ifdef NFS_PAGING
    nfspgiostart();
#endif

    /* start the frame manager process    */
    resume(fmgr = kcreate(PGRPLC, FMSTK, FMPRI, FMNAM, FMARGC));
}

/*-------------------------------------------------------------------------
 * pgioterminate - terminate the process or address space given
 *-------------------------------------------------------------------------
 */
pgioterminate(id)
     int id;
{
#ifdef XPP_PAGING
    return xpppgioterminate(id);
#endif
    
#ifdef NFS_PAGING
    return nfspgioterminate(id);
#endif
}


/*-------------------------------------------------------------------------
 * pgiocreate - create a new process or address space
 *-------------------------------------------------------------------------
 */
pgiocreate(id)
     int id;
{
#ifdef XPP_PAGING
    return xpppgiocreate(id);
#endif
    
#ifdef NFS_PAGING
    return nfspgiocreate(id);
#endif
}


/*-------------------------------------------------------------------------
 * pgiogetpage - get a page from the swap space
 *-------------------------------------------------------------------------
 */
pgiogetpage(id, findex, virt_addr)
{
#ifdef XPP_PAGING
    return xpppgiogetpage(id, findex, virt_addr);
#endif
    
#ifdef NFS_PAGING
    return nfspgiogetpage(id, findex, virt_addr);
#endif
}


/*-------------------------------------------------------------------------
 * pgiowakeup - wakeup a paging process, there is work to do
 *-------------------------------------------------------------------------
 */
pgiowakeup()
{
#ifdef XPP_PAGING
    send(sendproc, 0);
#endif

#ifdef NFS_PAGING
    signal(nfspgout_sem);
#endif
}
