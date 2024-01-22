/* x_pmem.c - x_pmem */


#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <vmem.h>
#include <a.out.h>


/*------------------------------------------------------------------------
 *  x_pmem - print the memory usage and stats for a process
 *------------------------------------------------------------------------
 */
COMMAND	x_pmem(stdin, stdout, stderr, argc, argv)
int	stdin, stdout, stderr, argc;
char	*argv[];
{
    int	id;
    int	pid;
    int	asid;
    struct	pentry *pentry;
    struct	aentry *aentry;
    struct	exec   *pexec;
    Bool	f_asid;
    int	pgs_mapped;
    int	pgs_inmem;
    unsigned int stack_top;
    unsigned int stack_bottom;
    unsigned int heap_top;
    unsigned int heap_bottom;


    if (argc != 2) {
	Usage(stderr,argv[0]);
	return(SYSERR);
    }

    id = atoi(argv[1]);
    if (ispid(id)) {
	if (proctab[id].pstate == PRFREE) {
	    fprintf(stderr, "bad pid: %d\n", id);
	    return(SYSERR);
	}
	pid = id;
	pentry = &proctab[pid];
	asid = pentry->asid;
	aentry = &addrtab[asidindex(pentry->asid)];
	f_asid = FALSE;
    } else if (isasid(id)) {
	if (!addrtab[asidindex(id)].valid) {
	    fprintf(stderr, "bad asid: %d\n", id);
	    return(SYSERR);
	}
	pid = BADPID;
	asid = id;
	aentry = &addrtab[asidindex(asid)];
	f_asid = TRUE;
    } else {
	Usage(stderr,argv[0]);
	return(SYSERR);
    }


    /* print main info */
    if (!f_asid) {
	fprintf(stdout, "process name: %s\n", pentry->pname);
	fprintf(stdout, "  id: %d\n", pid);
	fprintf(stdout, "  asid: %d\n", asid);
	fprintf(stdout, "  type: %s process\n",
		iskernproc(pid)?"kernel":"user");
	fprintf(stdout, "  entry point: 0x%x\n", pentry->paddr);
	fprintf(stdout, "  priority: %d\n", pentry->pprio);
	fprintf(stdout, "  next of kin: %d\n", pentry->pnxtkin);
	fprintf(stdout, "  timestamp: %d\n", pentry->tstamp);
	fprintf(stdout, "  state: ");
	switch (pentry->pstate) {
	  case PRCURR:	fprintf(stdout,"current\n"); break;
	  case PRFREE:	fprintf(stdout,"free\n"); break;
	  case PRREADY:	fprintf(stdout,"ready\n"); break;
	  case PRRECV:	fprintf(stdout,"receive\n"); break;
	  case PRSLEEP:	fprintf(stdout,"sleep\n"); break;
	  case PRSUSP:	fprintf(stdout,"suspended\n"); break;
	  case PRTRECV:	fprintf(stdout,"recvtim\n"); break;
	  case PRWAIT:	fprintf(stdout,"waiting on sem %d\n",
				pentry->psem); break;
	}
	if (pentry->phasmsg)
	    fprintf(stdout, "  message: %06x\n", pentry->pmsg);
    } else {
	fprintf(stdout, "  asid: %d\n", asid);
	fprintf(stdout, "  type: %s address space\n",
		(asid == XINUASID)?"kernel":"user");
    }

	
    /* get info about the size of text, data, and bss */
    if ((!f_asid && !iskernproc(pid)) || 
	(f_asid && asid != XINUASID)) {
	/* either an address space, or a user process */
	pexec = &aentry->aout;
	fprintf(stdout, "text size:    0x%08lx  0x%08lx-0x%08lx\n",
		pexec->a_text,
		0x2000,
		0x2000+pexec->a_text);
	fprintf(stdout, "data size:    0x%08lx  0x%08lx-0x%08lx\n",
		N_DATASIZE(*pexec),
		N_DATADDR(*pexec),
		N_DATADDR(*pexec)+pexec->a_data-1);
	fprintf(stdout, "bss size:     0x%08lx  0x%08lx-0x%08lx\n",
		N_BSSSIZE(*pexec),
		N_BSSADDR(*pexec),
		N_BSSADDR(*pexec)+pexec->a_bss-1);
	if (getumext(f_asid?asid:pid, &heap_bottom,&heap_top) == OK) {
	    heap_bottom = N_BSSADDR(*pexec)+pexec->a_bss;
	    fprintf(stdout, "heap range:   0x%08lx  0x%08lx-0x%08lx\n",
		    heap_top - heap_bottom + 1,
		    heap_bottom,
		    heap_top);
	}
    }

    /* get the stack info, if this is a process */
    if (!f_asid) {
	/* make sure that our registers are in the proc tab */
	if (pid == currpid)
	    ctxsw(pentry->pregs, pentry->pregs);

	stack_top = pentry->pbase;
	stack_bottom = pentry->pregs[SP];

	fprintf(stdout, "stack size:   0x%08lx  0x%08lx-0x%08lx\n",
		(stack_top - stack_bottom), stack_bottom, stack_top);
	fprintf(stdout, "  stack/rsa memory usage:\n");
	if (memusage(pid,vmaddrsp.minstk,vmaddrsp.rsaio,
		     &pgs_mapped,&pgs_inmem) == SYSERR)
	    fprintf(stdout, "  not available\n");
	else {
	    fprintf(stdout, "  used:      pages: %3d  bytes: 0x%0x\n",
		    pgs_mapped, pgs_mapped * PGSIZ);
	    fprintf(stdout, "  in memory: pages: %3d  bytes: 0x%0x\n",
		    pgs_inmem, pgs_inmem * PGSIZ);
	}
    }

    /* text,data,bss,heap memory usage, if asid or user process */
    if ((!f_asid && !iskernproc(pid)) || 
	(f_asid && asid != XINUASID)) {
	fprintf(stdout, "text,data,bss,heap memory usage:\n");
	if (memusage(pid,0x2000,vmaddrsp.maxheap,
		     &pgs_mapped,&pgs_inmem) == SYSERR)
	    fprintf(stdout, "  not available\n");
	else {
	    fprintf(stdout, "  used:      pages: %3d  bytes: 0x%0x\n",
		    pgs_mapped, pgs_mapped * PGSIZ);
	    fprintf(stdout, "  in memory: pages: %3d  bytes: 0x%0x\n",
		    pgs_inmem, pgs_inmem * PGSIZ);
	}
    }
    return(OK);
}


Usage(dev,prog)
     int dev;
     char *prog;
{
    fprintf(dev, "usage: %s pid\n", prog);
    fprintf(dev, "   print the memory usage of a process\n");
}

