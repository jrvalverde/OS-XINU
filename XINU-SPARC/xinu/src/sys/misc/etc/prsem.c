/* prsem.c - prsem */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>

/*------------------------------------------------------------------------
 * prsem  --  dump the semaphore table, or just one semaphore if 'sem' is
 *	      a valid semaphore id
 *------------------------------------------------------------------------
 */
prsem(sem)
     int sem;
{
	STATWORD ps;    
	int i,j,p;
	struct sentry *psem;

#ifdef SEM_DEBUG
	kprintf("id   state  count  pid-sign  pid-wait  waiting procs\n");
	kprintf("---  -----  -----  --------  --------  -------------\n");
#else
	kprintf("id   state  count  waiting procs\n");
	kprintf("---  -----  -----  -------------\n");
#endif

	disable(ps);
	for (i=0; i < NSEM; ++i) {
		if (!isbadsem(sem) && (sem != i))
		    continue;
		psem = &semaph[i];
		kprintf("%3d  %5s  ",
			i,
			(psem->sstate==SFREE)?"FREE":
			(psem->sstate==SUSED)?"USED":"BAD");
		if (psem->sstate == SFREE) {
			kprintf("\n");
			if (!isbadsem(sem))
			    continue;
			for (j=i+1; j < NSEM; ++j)
			    if (semaph[j].sstate != SFREE)
				break;
			/* i: index of first SFREE  j: index of first NUSED */
			if ((j - i) > 3) {	/* more than 3 in a row */
				kprintf("...\n");
				i = j-2;	/* jump to the last of them */
			}
			continue;
		}
		kprintf("%5d  ", psem->semcnt);
#ifdef SEM_DEBUG
		kprintf("%8u  ", psem->spidsig);
		kprintf("%8u  ", psem->spidwait);
#endif
		if (psem->semcnt >= 0) {
			kprintf("\n");
			continue;
		}
		for(p=q[psem->sqhead].qnext; p != psem->sqtail; p=q[p].qnext)
		    kprintf("%d (%s) ", p, proctab[p].pname);
		kprintf("\n");
	}
	restore(ps);
}
