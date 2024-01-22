/* tdump.c - tdump, tdumph, tdumpa */

#include <kernel.h>
#include <proc.h>
#include <q.h>

/*------------------------------------------------------------------------
 *  tdump  --  dump the active task table entries
 *------------------------------------------------------------------------
 */
tdump()
{
	txdump(0);
}

/*------------------------------------------------------------------------
 *  tdumph  --  dump the active talk table entries and halt
 *------------------------------------------------------------------------
 */
tdumph()
{
	txdump(0);
	kprintf("\nDump complete -- Exiting ...\n");
	halt();
}

/*------------------------------------------------------------------------
 *  tdumpa  --  dump the task table entries printing all entries
 *------------------------------------------------------------------------
 */
tdumpa()
{
	txdump(1);
}
static txdump(all)
int all;
{
        int i, j, ctr;
        long ps;
        struct pentry *pptr;
	long	*sp;

        disable(ps);
        kprintf("\nProctab at loc %o\ncurrpid is %d\n", proctab, currpid);
	for ( i=0 ; i<NPROC ; i++ ) {
            pptr = &proctab[i];
	    if (all!=0 || pptr->pstate!=PRFREE) {
                kprintf("\nProcess %d: ",i);
                kprintf("state=%o,name=",pptr->pstate);
                for ( j=0 ; j<PNMLEN ; j++ ) {
                        if (pptr->pname[j]=='\0') break;
                        kprintf("%c",pptr->pname[j]);
                }
                kprintf(",prio=%d,sem=%d,procaddr=%o\n",
			pptr->pprio,pptr->psem,pptr->paddr);
		stdump(i,"from tdump");
	    }
	}
	kprintf("rdyhead:");
	ctr = NPROC;
        for ( i = q[rdyhead].qnext; i < NPROC ; i=q[i].qnext) {
                kprintf("%d ",i);
		if (q[i].qnext == i) {
			kprintf("\nLOOP in ready list");
			break;
		}
		if (--ctr < 0) {
			kprintf("\nCorrupt ready list -- too long");
			break;
		}
	}
        kprintf("\n");
        restore(ps);
}
