/* 
 * printpgtbl.c - routine to print page tables
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
#include <frame.h>

/*---------------------------------------------------------------------------
 * printpgtbl - print (a portion) of the contents of a threads page table
 *---------------------------------------------------------------------------
 */
printpgtbl(pid, area, start, len)
int pid;
int area;
int start;
int len;
{
	struct  ppte    ppte;
	struct  ppte	*ppteptr;
	struct	pentry	*pptr;		/* pointer to process entry	*/
	char            *vaddr;
	int		stop;		/* last page to print		*/
	int		i,j,k;		/* counter variable		*/
	long		pgtbllen;	/* page table length		*/
	

	if (isbadpid(pid)) {
		kprintf("Process does not exist\n");
		return;
	}


	kprintf("\nThe area:%d page table for proc %d follows:\n\n",area, pid);
	
	switch (area) {
	case 1:
		start = topgnum(vmaddrsp.minaddr)+start;
		stop =(start+len<=(j=topgnum(vmaddrsp.maxheap)))?start+len:j;
		kprintf("(User text, data, bss, heap area)\n");
		break;
	case 2:
	        start = topgnum(vmaddrsp.stkstrt)- start;
		stop =(start-len >(j=topgnum(vmaddrsp.minstk)))? start-len:j;
		k = stop;  stop = start; start = k;
		kprintf("(User stack area)\n");
		break;
	case 3:
		start = topgnum(vmaddrsp.kernstk)- start;
		stop =(start-len>(j=topgnum(vmaddrsp.stkstrt)))? start-len:j+1;
		k = stop;  stop = start; start = k;
		kprintf("(Kernel stack area)\n");
		break;
	case 4:
	        start = topgnum(vmaddrsp.kernstk+4)+start;
		stop =(start+len < (j=topgnum(vmaddrsp.rsaio)))? start+len:j;
		kprintf("(RSA I/O area)\n");
		break;
	case 5:
		start = topgnum(vmaddrsp.rsaio+4)+start;
		stop =(start+len < (j=topgnum(vmaddrsp.maxsp)))? start+len:j;
		kprintf("(S/P area)\n");
		break;
	case 6:
		start = topgnum(vmaddrsp.xinu)+start;
		stop =(start+len<(j=topgnum(vmaddrsp.copyaddr)))?start+len:j-1;
		kprintf("(Xinu area)\n");
	}

	kprintf("---------------------------------------------------------------------------\n");
	kprintf("   p        p    z    g    i    l    o    t    m    m    h     p |  frame  \n");  
	kprintf("   a        f    e    a    n    i    n    v    b    o    v     r |l   s   u\n");
	kprintf("   g        n    r    r    m    s    a    a    z    d    a     o |o   h   s\n");
        kprintf("   e             o    b    e    t    c    l              l     t |c   a   e\n");
	kprintf("                           m         t    i              i       |k   r   d\n");
        kprintf("                                          d              d       |    e    \n");
	kprintf("-------  ------  --   --   --   --   --   --   --   --   --    --|--  --  -\n");
	        
	ppteptr = &ppte;
	vaddr   = (char *)(start*PGSIZ);
	getppte(pid, vaddr, ppteptr);
	for (i=start; i<= stop; i++) {
		pteprint(ppteptr, topgnum(vaddr));
		nextppte(pid, &vaddr, ppteptr);
	}
		
	kprintf("---------------------------------------------------------------------------\n");
}


/* 
 *--------------------------------------------------------------------------- 
 * pteprint - print a page table entry in readable format
 *--------------------------------------------------------------------------- 
*/
	
pteprint(ppteptr, pageno)
struct ppte *ppteptr;
int pageno;
{
	kprintf("%7x %7d %2d %4d %4d %4d %4d %4d %4d %4d %4d %5x %2d %3d %3d\n",
		pageno,
		(getinmem(ppteptr)==0)? 0 : frameindex(getpfn(ppteptr)),
		(getinmem(ppteptr)==1)? 0 : getzero(ppteptr),
		getinmem(ppteptr),
		(getinmem(ppteptr)? ftlist(frameindex(getpfn(ppteptr))):0),
		0, /* used to be getonactive */
		gettvalid(ppteptr),
		0, /* used to be getmbz */
		getmod(ppteptr),
		gethvalid(ppteptr),
		getprot(ppteptr),
	        (getinmem(ppteptr)==0)?
			0 : ftlocked(frameindex(getpfn(ppteptr))),
                (getinmem(ppteptr)==0)?
			0 : ftshared(frameindex(getpfn(ppteptr))),
		(getinmem(ppteptr)==0)?
			0 : ftused(frameindex(getpfn(ppteptr))));
}

