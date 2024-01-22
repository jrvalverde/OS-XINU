
/* 
 * printfunctions.c - Functions that print various kernel data structures
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri May 13 11:09:08 1988
 *
 * Copyright (c) 1988 Jim Griffioen
 */


#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <mem.h>
#include <tty.h>
#include <q.h>
#include <io.h>
#include <network.h>
#include <sleep.h>
#include <vmem.h>
#include <pgio.h>


/*---------------------------------------------------------------------------
 * prvmaddrsp - print the virtual address space layout
 *---------------------------------------------------------------------------
 */
prvmaddrsp()
{
	kprintf("vmaddrsp.minaddr = %09x\n", (unsigned)vmaddrsp.minaddr);
	kprintf("vmaddrsp.maxheap = %09x\n", (unsigned)vmaddrsp.maxheap);
	kprintf("vmaddrsp.minstk = %09x\n", (unsigned)vmaddrsp.minstk);
	kprintf("vmaddrsp.stkstrt = %09x\n", (unsigned)vmaddrsp.stkstrt);
	kprintf("vmaddrsp.kernstk = %09x\n", (unsigned)vmaddrsp.kernstk);
	kprintf("vmaddrsp.rsaio = %09x\n", (unsigned)vmaddrsp.rsaio);
	kprintf("vmaddrsp.maxsp = %09x\n", (unsigned)vmaddrsp.maxsp);
	kprintf("vmaddrsp.xinu = %09x\n", (unsigned)vmaddrsp.xinu);
	kprintf("vmaddrsp.devio = %09x\n", (unsigned)vmaddrsp.devio);
	kprintf("vmaddrsp.pgtbl = %09x\n", (unsigned)vmaddrsp.pgtbl);
	kprintf("vmaddrsp.ftbl = %09x\n", (unsigned)vmaddrsp.ftbl);
	kprintf("vmaddrsp.xheapmin = %09x\n", (unsigned)vmaddrsp.xheapmin);
	kprintf("vmaddrsp.xheapmax = %09x\n", (unsigned)vmaddrsp.xheapmax);
	kprintf("vmaddrsp.copyaddr = %09x\n", (unsigned)vmaddrsp.copyaddr);
	kprintf("vmaddrsp.copysize = %09x\n", (unsigned)vmaddrsp.copysize);
}



#ifdef UVAX

/*---------------------------------------------------------------------------
 * prproccb - print process control block on the Micro Vax I
 *---------------------------------------------------------------------------
 */
prproccb(pid)
int pid;
{
	struct pentry *pptr;		/* ptr to process table entry	*/

	pptr = (struct pentry *) &proctab[pid];
	kprintf("k stk = %08x, e stk = %08x, s stk = %08x, u stk = %08x\n",
		(unsigned)pptr->pregs[0],(unsigned)pptr->pregs[1],
		(unsigned)pptr->pregs[2],(unsigned)pptr->pregs[3]);
	kprintf("r0 = %08x, r1 = %08x, r2 = %08x, r3 = %08x\n",
		(unsigned)pptr->pregs[4],(unsigned)pptr->pregs[5],
		(unsigned)pptr->pregs[6],(unsigned)pptr->pregs[7]);
	kprintf("r4 = %08x, r5 = %08x, r6 = %08x, r7 = %08x\n",
		(unsigned)pptr->pregs[8],(unsigned)pptr->pregs[9],
		(unsigned)pptr->pregs[10],(unsigned)pptr->pregs[11]);
	kprintf("r8 = %08x, r9 = %08x, r10 = %08x, r11 = %08x\n",
		(unsigned)pptr->pregs[12],(unsigned)pptr->pregs[13],
		(unsigned)pptr->pregs[14],(unsigned)pptr->pregs[15]);
	kprintf("ap = %08x, fp = %08x, pc = %08x, psl = %08x\n",
		(unsigned)pptr->pregs[16],(unsigned)pptr->pregs[17],
		(unsigned)pptr->pregs[18],(unsigned)pptr->pregs[19]);
	kprintf("P0BR = %08x, P0LR = %08x, P1BR = %08x, P1LR = %08x\n",
		(unsigned)pptr->pregs[20],(unsigned)pptr->pregs[21],
		(unsigned)pptr->pregs[22],(unsigned)pptr->pregs[23]);
}

#endif UVAX

