/* 
 * printheap.c - routine to print free heap memory list
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Dec  3 1987
 * Copyright (c) 1987 Jim Griffioen
 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mem.h>
#include <vmem.h>
#ifdef UVAX
#include <procreg.h>
#endif UVAX

/*---------------------------------------------------------------------------
 * printheap - print the free heap list for the process pid
 *---------------------------------------------------------------------------
 */
printheap(pid)
int pid;
{
	struct mblock *p;
	int	i;
	int	asid, asindx;

	if (isbadpid(pid)) {
		kprintf("printheap: bad pid = %d\n", pid);
		return(SYSERR);
	}
	asid = proctab[pid].asid;
	asindx = asidindex(asid);

	kprintf("\nThe freelist for proc %d in address space %d follows:\n",
		pid, asid);

	p = vmemlist[asindx].mnext;
	if (p == (struct mblock *) NULL) {
		kprintf("This process has no P0 free space\n");
		return;
	}
	i = 0;
	while (p != (struct mblock *)NULL) {
		kprintf("block[%d],  start addr = %08x,  len = %d\n",
			i++, p, p->mlen);
		p = p->mnext;
	}
}

