/* pmegdump.c - pmegdump */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <vmem.h>
#include <frame.h>
#include <proc.h>
#include <sem.h>


/*------------------------------------------------------------------------
 *  pmegdump - dump the list of pmegs.  Output device is 'dev'.  If 'pid'
 *  is not (-1), then only print the pmegs allocated to that pid.
 *------------------------------------------------------------------------
 */
pmegdump(dev,pid)
     int dev;
     int pid;
{
    int	i;
    struct	pmege *pmege;
    
    dprintf(dev,"\n      pmeg table\n\n");
    dprintf(dev,"num  status  pid  addr range       \n");
    dprintf(dev,"___  ______  ___  _________________\n");
    
    for (i=0; i < VM_NUM_PMEGS; ++i) {
	pmege = &pmegtbl[i];
	if ((pid == -1) || (pid == pmege->pid)) {
	    dprintf(dev,"%3d  ", pmege->num);
	    switch (pmege->stat) {
	      case VM_PMEG_LOCKED:
		dprintf(dev,"LOCKED\n");
		break;
	      case VM_PMEG_FREE:
		dprintf(dev,"FREE\n");
		break;
	      case VM_PMEG_INUSE:
		dprintf(dev,"IN_USE  %3d  %08x-%08x\n",
			pmege->pid,
			pmege->vaddr,
			pmege->vaddr + 0x20000 - 1);
		break;
	    }
	}
	
    }
    
    dprintf(dev,"\n");
    
    if (pid != -1)
	return;
    
    dprintf(dev,"\nPMEG free list:\n\thead");
    pmege = (&(pmegtbl[VM_PMEG_FREEHD]))->next;
    i = 0;
    while (pmege != (&(pmegtbl[VM_PMEG_FREETL]))) {
	if (++i % 10 == 0) dprintf(dev,"\n\t");
	dprintf(dev,"->%d", pmege->num);
	pmege = pmege->next;
    }
    dprintf(dev,"->tail\n");
    dprintf(dev,"\nPMEG in-use list:\n\thead");
    pmege = (&(pmegtbl[VM_PMEG_LISTHD]))->next;
    i = 0;
    while (pmege != (&(pmegtbl[VM_PMEG_LISTTL]))) {
	if (++i % 10 == 0) dprintf(dev,"\n\t");
	dprintf(dev,"->%d", pmege->num);
	pmege = pmege->next;
    }
    dprintf(dev,"->tail\n");
}



/* for debugging after a crash, uses kprintf instead */
dbgpmeg()
{
    pmegdump(-1,-1);
}
