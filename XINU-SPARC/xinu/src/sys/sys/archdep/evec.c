/* evec.c -- initevec, doevec */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <mem.h>
#include <vmem.h>
#include <tty.h>
#include <q.h>
#include <io.h>
#include <interreg.h>

/*#define DEBUG*/
/*#define STACKTRACE_DEBUG*/

#define NUMEVEC 256    

/* the Xinu exception vector */    
unsigned long  evec[NUMEVEC];

char *origevec;

int doevec();


/*-------------------------------------------------------------------------
 * initevec - 
 *-------------------------------------------------------------------------
 */
initevec()
{
    int e;
    
    for (e = 2; e < NUMEVEC; ++e)
	set_evec(e*4, doevec);
}


/*-------------------------------------------------------------------------
 * set_evec - 
 *-------------------------------------------------------------------------
 */
set_evec(offset, funct)
     int offset;
     unsigned int funct;
{
    unsigned int oldvalue;
    
#ifdef DEBUG
    kprintf("set_evec(vector=%d, funct=0x%08x)\n", offset, funct);
#endif
    oldvalue = (unsigned int) evec[offset/4];
    evec[offset/4] = (unsigned long) funct;
    return(oldvalue);
}


/*-------------------------------------------------------------------------
 * doevec - 
 *-------------------------------------------------------------------------
 */
doevec(evnum, pc, npc)
     int evnum;
     unsigned long pc, npc;
{
    int notdone;
    
    kprintf("\nXinu Trap!  ");
    kprintf("exception: 0x%x  ", evnum);
    kprintf("pc: 0x%lx  ", pc);
    kprintf("npc: 0x%lx\n", npc);
    kprintf("Trap definition: ");
    
    notdone = 0;
    switch(evnum) {
      case  0: kprintf("RESET0, SHOULDN'T SEE THIS\n"); break;
      case  1: kprintf("Instruction Access Exception\n"); break;
      case  2: kprintf("Illegal Instruction\n"); break;
      case  3: kprintf("Privileged Instruction\n"); break;
      case  4: kprintf("fp_disabled\n"); break;
      case 36: kprintf("cp_disabled\n"); break;
      case  5: kprintf("Window Overflow-SHOULDN'T SEE THIS\n"); break;
      case  6: kprintf("Window Underflow-SHOULDN'T SEE THIS\n"); break;
      case  7: kprintf("Memory Address Not Aligned\n"); break;
      case  8: kprintf("fp_exception\n"); break;
      case 40: kprintf("cp_exception\n"); break;
      case  9: kprintf("Data Access Exception\n"); break;
      case 10: kprintf("Tag Overflow\n"); break;
	
      case 17: kprintf("Interrupt Level 1\n"); break;
      case 18: kprintf("Interrupt Level 2\n"); break;
      case 19: kprintf("Interrupt Level 3\n"); break;
      case 20: kprintf("Interrupt Level 4\n"); break;
      case 21: kprintf("Interrupt Level 5\n"); break;
      case 22: kprintf("Interrupt Level 6\n"); break;
      case 23: kprintf("Interrupt Level 7\n"); break;
      case 24: kprintf("Interrupt Level 8\n"); break;
      case 25: kprintf("Interrupt Level 9\n"); break;
      case 26: kprintf("Interrupt Level 10\n"); break;
      case 27: kprintf("Interrupt Level 11\n"); break;
      case 28: kprintf("Interrupt Level 12\n"); break;
      case 29: kprintf("Interrupt Level 13\n"); break;
      case 30: kprintf("Interrupt Level 14 - clock\n"); break;
      case 31: kprintf("Interrupt Level 15\n"); break;
      case 130: kprintf("Divide by 0\n"); break;
      default: notdone = 1; break;
    }
    
    if ((evnum >= 128) && (evnum <= 255))
	kprintf("Trap Instruction (Ticc): trap vector = %d\n",evnum);
    else if (notdone)
	kprintf("Unknown Interrupt Vector: trap vector = %d\n", evnum);
    
    kprintf("\n");
#ifdef STACKTRACE_DEBUG
    stacktrace();
#endif
    evecnocont();
}

/* We want to keep people from continuing when an exception occurs */

/*-------------------------------------------------------------------------
 * evecnocont - 
 *-------------------------------------------------------------------------
 */
LOCAL evecnocont()
{
    while (TRUE) {
	ret_mon();
	kprintf("Can't continue from exception\n");
    }
}

