/* stktrace.c - print out a stack trace on the SPARC */

#include <conf.h>
#include <kernel.h>
#include <vmem.h>
#include <proc.h>

extern char *etext;

#define PRINTF	kprintf

unsigned int stacktrace_fp;
unsigned int stacktrace_sp;

/*---------------------------------------------------------------------------
 * stacktrace - Print a stack trace for the current process
 *---------------------------------------------------------------------------
 */
stacktrace()
{
    int i, j, k;
    int *sp;
    int *pc;
    int *fp;
    int *stkbase;

    /* get the current fp and sp */
    asm("set _stacktrace_fp, %o4");
    asm("st  %fp, [%o4]");
    asm("set _stacktrace_sp, %o4");
    asm("st  %sp, [%o4]");

    /* now make sure that everything is really in memory */
    asm("save; save; save; save; save; save; save");
    asm("restore; restore; restore; restore");
    asm("restore; restore; restore");

    fp = (int *) stacktrace_fp;		/* this is not needed, can get from stk */
    sp = (int *) stacktrace_sp;
    stkbase = (int *) vmaddrsp.kernstk;
    pc = 0;
	
    PRINTF("Stack Trace Back:\n");
    PRINTF("----------------\n");
    PRINTF("Stack Base = 0x%08x\n", stkbase);
    while ((sp < stkbase) && (sp > vmaddrsp.minstk)) {
	PRINTF("PC: 0x%08lx ", pc); whereis(pc);
	PRINTF("SP: 0x%08lx ", sp); whereis(sp);
	fp = (int *) *(sp+14);
	PRINTF("FP: 0x%08lx ", fp); whereis(fp);
	PRINTF("Locals:\n");
	for (i=0; i < 8; ++i) {
	    PRINTF("  %%l%d: 0x%08lx  ",
		   i, *(sp+i)); whereis(*(sp+i));
	}
	PRINTF("Ins:\n");
	for (i=8; i < 16; ++i) {
	    PRINTF("  %%i%d: 0x%08lx  ",
		   i-8, *(sp+i)); whereis(*(sp+i));
	}
	PRINTF("\n");
		
	pc = (int *) *(sp+15);
	sp = (int *) *(sp+14);
    }
}


/*---------------------------------------------------------------------------
 * whereis - print what part of the address space an address is in
 *---------------------------------------------------------------------------
 */
whereis(vaddr)
int vaddr;
{
    if (vaddr > (int) vmaddrsp.xheapmax)
	PRINTF("beyond kernel heap\n");
    else if (vaddr > (int) vmaddrsp.xheapmin)
	PRINTF("kernel heap area\n");
    else if (vaddr > (int) vmaddrsp.ftbl)
	PRINTF("kernel frame table area\n");
    else if (vaddr > (int) &etext)
	PRINTF("kernel data area\n");
    else if (vaddr > (int) vmaddrsp.xinu)
	PRINTF("kernel text address\n");
    else if (vaddr > (int) vmaddrsp.maxsp)
	PRINTF("between maxsp and start of xinu text\n");
    else if (vaddr > (int) vmaddrsp.rsaio)
	PRINTF("between rsaio address and kernel text\n");
    else if (vaddr > (int) vmaddrsp.kernstk)
	PRINTF("rsaio address\n");
    else if (vaddr > (int) vmaddrsp.stkstrt)
	PRINTF("kernstk area\n");
    else if (vaddr > (int) vmaddrsp.minstk)
	PRINTF("user stack address\n");
    else if (vaddr > (int) vmaddrsp.maxheap)
	PRINTF("no mans land after heap\n");
    else if (vaddr > (int) vmaddrsp.minaddr)
	PRINTF("just a number - or user heap address\n");
    else
	PRINTF("unknown number\n");
}

