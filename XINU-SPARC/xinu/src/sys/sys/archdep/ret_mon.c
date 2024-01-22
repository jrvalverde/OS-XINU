/*------------------------------------------------------------------------
 *	ret_to_monitor -- return to the Sun monitor
 *------------------------------------------------------------------------
 */

#include <conf.h>
#include <kernel.h>
#include <openprom.h>
#include <vmem.h>

#define DUMP_MEMORY_DEBUG_INFO

/*
 *   Return to the Sun monitor program. 
 *
 *   Currently called by:
 *     ttyiin -- on BREAK
 *     doevec -- when exception occurs
 *     xdone  -- when all user processes have finished
 */


/*-------------------------------------------------------------------------
 * ret_mon - 
 *-------------------------------------------------------------------------
 */
ret_mon()
{
#ifdef DUMP_MEMORY_DEBUG_INFO
    PrDebug();				/* print out memory debugging info */
#endif
    /* (romp->v_exit_to_mon)();		/* exit to the monitor		*/
    (romp->v_abortent)();		/* break to the monitor		*/
}


/*-------------------------------------------------------------------------
 * reboot_machine - 
 *-------------------------------------------------------------------------
 */
reboot_machine()
{
   for(;;)
       (romp->v_boot_me)();		/* Never coming back		*/
}


