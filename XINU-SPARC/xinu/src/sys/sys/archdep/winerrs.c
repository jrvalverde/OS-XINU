/* winerrs.c - routines for handling serious over/under flow window intrpts */

#include <kernel.h>
#include <traps.h>
#include <proc.h>

/* Routines for the kernel window o/u flow handlers to call if there is	*/
/* a serious error.							*/

/*-------------------------------------------------------------------------
 * win_oflow_error - 
 *-------------------------------------------------------------------------
 */
win_oflow_error(pc, npc, sp)
     char *pc, *npc, *sp;
{
    kprintf("*** Kernel Stack overflow error (currpid=%d):\n", currpid);
    kprintf("    pc = 0x%08x, npc = 0x%08x, sp = 0x%08x\n", pc, npc, sp);
    kprintf("    One of 3 things was wrong:\n");
    kprintf("       1) sp not aligned correctly\n");
    kprintf("       2) sp page not (read/write)able\n");
    kprintf("       3) sp + WINDOW_SAVE_SIZE page not (read/write)able\n");
    kprintf("    Note: Kernel stacks are supposed to be locked in \n");
    kprintf("          so 2 and 3 should not be problems!\n");
    panic("Kernel Stack Overflow Error: halting the system");
}


/*-------------------------------------------------------------------------
 * win_uflow_error - 
 *-------------------------------------------------------------------------
 */
win_uflow_error(pc, npc, sp)
     char *pc, *npc, *sp;
{
    kprintf("*** Kernel Stack underflow error (currpid=%d):\n", currpid);
    kprintf("    pc = 0x%08x, npc = 0x%08x, sp = 0x%08x\n", pc, npc, sp);
    kprintf("    One of 3 things was wrong:\n");
    kprintf("       1) sp not aligned correctly\n");
    kprintf("       2) sp page not (read/write)able\n");
    kprintf("       3) sp + WINDOW_SAVE_SIZE page not (read/write)able\n");
    kprintf("    Note: Kernel stacks are supposed to be locked in \n");
    kprintf("          so 2 and 3 should not be problems!\n");
    panic("Kernel Stack Underflow Error: halting the system");
}
