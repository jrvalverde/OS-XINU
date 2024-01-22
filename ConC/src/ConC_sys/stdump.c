/* stdump.c - stdump */

#include <kernel.h>
#include <proc.h>
#include <q.h>

long *stdump_getap(), *stdump_getfp(), *stdump_getsp();

stdump(tid,sb)
int tid;
char *sb;
{
    long *sp, *ap, *fp;
    long ps;

    if (tid == gettid()) {
	ap = stdump_getap();
	fp = stdump_getfp();
	sp = stdump_getsp();
	disable(ps);				/* get PS into ps */
	restore(ps);
	}
    else {
	ap = (long *)proctab[tid].pregs[AP];
	fp = (long *)proctab[tid].pregs[FP];
	sp = (long *)proctab[tid].pregs[SP];
	ps = (long  )proctab[tid].pregs[PS];
	}

    kprintf("Stack dump for process #%d (name '%s') (%s):\n",
				tid,proctab[tid].pname,sb);
    kprintf("\tAP = %12o\tFP = %12o\tSP = %12o\n\tPS = %12o\n",ap,fp,sp,ps);
    kprintf("\tpbase = %12o\tplimit = %12o\n\n",
				proctab[tid].pbase,proctab[tid].plimit);
    kprintf("\tpargs = %12o\t paddr= %12o\n\n",
				proctab[tid].pargs,proctab[tid].paddr);
    for ( ; sp <= (long *)proctab[tid].pbase ; sp++ )
	kprintf("\t%12o\n",*sp);
}
