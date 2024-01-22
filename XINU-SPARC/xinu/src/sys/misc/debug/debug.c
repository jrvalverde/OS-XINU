
/* 
 * debug.c - This routine keeps debugging info in a string that can be
 * 	     be printed or can be displayed after a crash
 *	     Use the /bin/nm program to locate the address PrDebug, then
 *	     use the sun PROM monitor to start executing at PrDebug
 *	     when the system crashes.  This routine is very handy for
 *	     debugging time critical routines such as interrupt handlers.
 */


#include <conf.h>
#include <kernel.h>


#define DEBUG
/*#define PRINT_WRAP_AROUND*/

/* define only one of the following */
/*#define PRINT_ALL*/
#define PRINT_TAIL_ONLY

#define DEBUGBUFSIZE (30000)

#ifdef DEBUG
int debugptr_initialized = 0;
char *debugptr = 0;
char debugbuf[DEBUGBUFSIZE+50];
#endif



/*---------------------------------------------------------------------------
 * Debug - Save debugging info in a string
 *---------------------------------------------------------------------------
 */
Debug(str)
     char *str;
{
#ifdef DEBUG
	PStype ps;

	disable(ps);
	/* initialize the pointer */
	if (! debugptr_initialized) {	
		debugptr = debugbuf;
		debugptr_initialized = 1;
	}
	while (*str != 0) {
		if ((unsigned) debugptr > ((unsigned)debugbuf) + DEBUGBUFSIZE) {
#ifdef PRINT_WRAP_AROUND
			kprintf("debugptr wrap around\n");
#endif
			debugptr = debugbuf;
		}
		*debugptr++ = *str++;
	}
	restore(ps);
#endif
}



/*---------------------------------------------------------------------------
 * PrDebug - Print Debugging string - useful after a crash
 *---------------------------------------------------------------------------
 */
PrDebug()
{
#ifdef DEBUG
#ifdef PRINT_ALL
	DebugDumpAll();
#endif
#ifdef PRINT_TAIL_ONLY
	DebugDumpTail();
#endif
#endif	
}

DebugDumpAll()
{
#ifdef DEBUG	
	char * i;
	int j;

	kprintf("Debug log (called from ret_mon):\n");

	for (j=0, i=debugbuf; i < debugptr; ++i, j++) {
		kputc(CONSOLE, *i);
		if (j == 79){
			kputc(CONSOLE, 10);
			kputc(CONSOLE, 13);
			j = 0;
		}
	}
	kprintf("\nEnd of Debug Log\n");
#endif	
}

DebugDumpTail()
{
#ifdef DEBUG
	char * i;
	int j;

	kprintf("Debug log - tail info only (called from ret_mon):\n");

	for (j=0, i=(debugptr-1000);
	     i < debugptr;
	     ++i, j++) {
		kputc(CONSOLE, *i);
		if (j == 79){
			kputc(CONSOLE, 10);
			kputc(CONSOLE, 13);
			j = 0;
		}
	}
	kprintf("\nEnd of Debug Log\n");
#endif	
}

