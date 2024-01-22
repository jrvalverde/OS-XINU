/* int_demux.c - dispatch interrupts to correct routine */

#include <traps.h>

/*#define DEBUG*/
#define DEBUG_SPURIOUS_INTERRUPTS
/*#define MEM_DEBUGGING*/

/* the Xinu exception vector */    
extern int (*evec[])();

/*---------------------------------------------------------------------------
 * int_demux - dispatch interrupts to correct routine
 *---------------------------------------------------------------------------
 */
int_demux(vector, excptn_ptr)
unsigned int vector;
struct exception_frame *excptn_ptr;
{
	int	(*procptr)();

#ifdef MEM_DEBUGGING
	int val;
	if (vector == 30)
	    Debug("<C");
	else if (vector == 28)
	    Debug("<T");
	else if (vector == 21)
	    Debug("<E");
	else if (vector == 9)
	    Debug("<D");
	else if (vector == 1)
	    Debug("<I");
	else if (vector == 130) {
		Debug("<Z");
		kprintf("\nint_demux: v = %d, pc = %x, npc = %x\n",
			vector, excptn_ptr->pc, excptn_ptr->npc);
		kprintf("int_demux: about to call %x\n", evec[vector]);
	}
	else
	    Debug("<U");
#endif
#ifdef DEBUG
if ((vector != 30) && (vector != 28)) {		/* ignore clock and tty */
	kprintf("int_demux: v = %d, pc = %x, npc = %x\n",
		vector, excptn_ptr->pc, excptn_ptr->npc);
	kprintf("int_demux: about to call %x\n", evec[vector]);
}
#endif

#ifdef DEBUG_SPURIOUS_INTERRUPTS
{
extern long etext;

	if (((vector > 30) && (vector < 128)) ||
	    (vector > 147)) {
		kprintf("int_demux: spurious interrupt %d, tt addr = %x\n",
			vector, &(evec[vector]));
		kprintf("int_demux: about to call %x\n", evec[vector]);
	}
	if ( ((unsigned) evec[vector] < 0xfd04000) ||
	     ((unsigned) evec[vector] > (unsigned) &etext) ) {
		kprintf("int_demux: spurious interrupt %d, tt addr = %x\n",
			vector, &(evec[vector]));
		kprintf("int_demux: about to call %x\n", evec[vector]);
	}
}
#endif

#ifdef MEM_DEBUGGING
        val = (*evec[vector])(vector, excptn_ptr);
	if (vector == 30)
	    Debug("C>");
	else if (vector == 28)
	    Debug("T>");
	else if (vector == 21)
	    Debug("E>");
	else if (vector == 9)
	    Debug("D>");
	else if (vector == 1)
	    Debug("I>");
	else if (vector == 130) 
	    Debug("Z>");
	else
	    Debug("U>");
	return(val);
#else
        return( (*evec[vector])(vector, excptn_ptr) );
#endif
}
