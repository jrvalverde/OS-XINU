/* initialize.c - nulluser, sysinit */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <sem.h>
#include <sleep.h>    
#include <mem.h>
#include <tty.h>
#include <q.h>
#include <io.h>
#include <network.h>
#include <vmem.h>
#include <frame.h>

/*#define DEBUG*/
/*#define DEBUG_PARSE_ARGS*/
/*#define DETAIL*/

extern	int	main();			/* address of user's main prog	*/
extern	long	edata, etext;
extern	int	start();		/* startup code */

/* Declarations of major kernel variables */
struct	aentry	addrtab[NADDR];	/* address space table			*/
int	nextaddr;		/* next address space slot to use	*/
struct	pentry	proctab[NPROC]; /* process table			*/
int	nextproc;		/* next process slot to use in create	*/
struct	sentry	semaph[NSEM];	/* semaphore table			*/
int	nextsem;		/* next semaphore slot to use in screate*/
struct	qent	q[NQENT];	/* q table (see queue.c)		*/
int	nextqueue;		/* next slot in q structure to use	*/
int	xkernsize;		/* size of xinu txt,data,bss, and heap	*/
int 	machinesid;		/* machine system identification	*/
struct	pginfo	pg;		/* page table information		*/
struct	ftinfo	ftinfo;		/* frame table information		*/
struct	fminfo	fminfo;		/* frame manager information		*/
struct	vmaddrsp vmaddrsp;	/* vm address space information table	*/
union	pte	*syspgtbl;	/* addr of sys pg tbl (set by startup)	*/
struct	fte	*ft;		/* address of frame table		*/
int	ftfreesem;		/* free frame semaphore			*/
long	prloldest;		/* oldest timestamp on pending list	*/
int	prlsize;		/* size of pending list			*/
int	msgcount;		/* message counter for msg number	*/
int	msgpool;		/* message buf pool id			*/
int	nodepool;		/* request node buf pool id		*/
char	*rbuf;			/* receiving buffer			*/
char	*rmsg;			/* receiving message buffer		*/
struct	fte *flmptr;		/* pointer to fte on M-list		*/
char    machineid[4];		/* copy my IP addr into this variable	*/
long	cpudelay = 3;		/* for delaying execution		*/

#ifdef	NDEVS
struct	intmap	intmap[NDEVS];	/* interrupt dispatch table		*/
#endif

struct	mblock	memlist;	/* list of free memory blocks		*/
struct	mblock	vmemlist[NADDR];/* list of free mem in p0 space		*/

#ifdef	Ntty
struct  tty     tty[Ntty];	/* SLU buffers and mode control		*/
#endif

/* active system status */

int	numproc;		/* number of live user processes	*/
int	numaddr;		/* number of live address spaces	*/
int	currpid;		/* id of currently running process	*/
int	reboot = 0;		/* non-zero after first boot		*/

int	rdyhead,rdytail;	/* head/tail of ready list (q indicies)	*/
char	*sysid;
char	vers[] = VERSION;	/* Xinu version printed at startup	*/

/* These variables were defined in usrmain. */
/*
int	sem;
int	pid1, pid2;
int	ptid;
*/
/************************************************************************/
/***				NOTE:				      ***/
/***								      ***/
/***   This is where the system begins after the C environment has    ***/
/***   been established.  Interrupts are initially DISABLED, and      ***/
/***   must eventually be enabled explicitly.  This routine turns     ***/
/***   itself into the null process after initialization.  Because    ***/
/***   the null process must always remain ready to run, it cannot    ***/
/***   execute code that might cause it to be suspended, wait for a   ***/
/***   semaphore, or put to sleep, or exit.  In particular, it must   ***/
/***   not do I/O unless it uses kprintf for polled output.           ***/
/***								      ***/
/************************************************************************/

/*------------------------------------------------------------------------
 *  nulluser  -- initialize system and become the null process (id==0)
 *------------------------------------------------------------------------
 */
nulluser()				/* babysit CPU when no one home */
{
    int userpid;
    struct counterregs *timer_chip;

    timer_chip = (struct counterregs *)((struct intersil7170 *) CLOCK_REG);

    kprintf("\n\n%s", vers);
    if (reboot++ < 1)
	kprintf("\n");
    else
	kprintf("   (reboot %d)\n", reboot);
#ifdef DEBUG
    kprintf("Calling initevec\n");
#endif

    /* initevec must come before sysinit() since sysinit()	*/
    /* sets (to the correct value) some interrupt vectors	*/
    initevec();

#ifdef DEBUG
    kprintf("Calling sysinit\n");
#endif
    sysinit();

#ifdef DETAIL	
    kprintf("0x%08x (%d) bytes real mem\n", (unsigned long) physmemsize,
	    (unsigned long) physmemsize);			
#endif	

    kprintf("kernel size = %d, ", (unsigned long) xkernsize);
    kprintf("%u available real mem\n", (unsigned)(mem_size - xkernsize));
    kprintf("%d bytes text, data, bss, %d bytes heap\n",
	    (unsigned long) ((unsigned long)&end -
			     (unsigned long) vmaddrsp.xinu),
	    (unsigned long) ((unsigned long) vmaddrsp.xheapmax -
			     (unsigned long) vmaddrsp.xheapmin));

#ifdef DETAIL	
    kprintf("    0x%08lx", (unsigned long) start);
    kprintf(" to 0x%08lx\n", (unsigned long) &end );
#endif

    enable();		/* enable interrupts */

#ifdef DEBUG
    kprintf("returned from enabling interrupts\n");
#endif

#ifdef DEBUG
    kprintf("Now we will start creating user process\n");
    kprintf("creating the main() process\n");
#endif
    /* create a process to execute the user's main program */
    userpid = kcreate(main, INITSTK, INITPRIO, INITNAME, INITARGS);

#ifdef DEBUG
    kprintf("creating netin\n");
#endif

    /* create network servers and then resume "main" */
    resume(kcreate(netstart, NETSTK, NETPRI, NETNAM, NETARGC, userpid));

    while (TRUE) {		/* run forever without actually */
	pause();		/* executing instructions */
    }
}

/*------------------------------------------------------------------------
 *  sysinit  --  initialize all Xinu data structeres and devices
 *------------------------------------------------------------------------
 */
LOCAL	sysinit()
{
    int	i;
    struct	aentry	*aptr;		/* temp address space entry ptr	*/
    struct	pentry	*pptr;		/* temp process entry ptr	*/
    struct	sentry	*sptr;		/* temp semephore entry ptr	*/
    struct	mblock	*mptr;		/* temp mem block ptr		*/
    int	findex;			/* temp frame index variable	*/
    struct  ppte    ppte;
    struct  ppte    *ppteptr;
    char	*vaddr;			/* temp virtual address		*/
    int	dsptcher(), kernret();

    numproc  = 0;			/* initialize system variables */
    nextproc = NPROC-1;
    nextsem  = NSEM-1;
    nextqueue= NPROC;		/* q[0..NPROC-1] are processes */

    /*
     * configure system parameters using the PROM monitor
     */
    config_system();

#ifdef DEBUG
    kprintf("Initializing semaphores\n");
#endif	

    /* initialize semephores before calling vminit because frame	*/
    /* table initialization needs semephore				*/
    for (i=0 ; i<NSEM ; i++) {	/* initialize semaphores 	*/
	(sptr = &semaph[i])->sstate = SFREE;
	sptr->sqtail = 1 + (sptr->sqhead = newqueue());
    }

#ifdef DEBUG
    kprintf("calling vminit\n");
#endif
    /* init virtual memory data structs and turn on virt mem	*/
    vminit();			

#ifdef DETAIL
    kprintf("\n");

    kprintf("    User TDBH     (%d bytes)\n",
	    (u_long) vmaddrsp.maxheap - (u_long) vmaddrsp.minaddr);
    kprintf("range	: 0x%08lx - 0x%08lx\n", (u_long) vmaddrsp.minaddr,
	    (u_long) vmaddrsp.maxheap);
    kprintf("    NO MAN'S LAND\n");
    kprintf("range	: 0x%08lx - 0x%08lx\n", (u_long) vmaddrsp.maxheap,
	    (u_long) vmaddrsp.minstk);
    kprintf("    User Stack    (%d bytes)\n",
	    (u_long) vmaddrsp.stkstrt - (u_long) vmaddrsp.minstk);
    kprintf("range	: 0x%08lx - 0x%08lx\n", (u_long) vmaddrsp.minstk,
	    (u_long) vmaddrsp.stkstrt);
    kprintf("    KERNEL Stack  (%d bytes)\n",
	    (u_long) vmaddrsp.kernstk - (u_long) vmaddrsp.stkstrt);
    kprintf("range	: 0x%08lx - 0x%08lx\n", (u_long) vmaddrsp.stkstrt,
	    (u_long) vmaddrsp.kernstk);
    kprintf("    RSA           (%d bytes)\n",
	    (u_long) vmaddrsp.rsaio - (u_long) vmaddrsp.kernstk - 1);
    kprintf("range	: 0x%08lx - 0x%08lx\n", (u_long) vmaddrsp.kernstk,
	    (u_long) vmaddrsp.rsaio);
    kprintf("    OTHER STUFF   (%d bytes)\n",
	    (u_long) vmaddrsp.xinu - (u_long) vmaddrsp.rsaio);
    kprintf("range	: 0x%08lx - 0x%08lx\n", (u_long) vmaddrsp.rsaio,
	    (u_long) vmaddrsp.xinu);
    kprintf("    XINU TEXT     (%d bytes)  \n",
	    (u_long) &etext - (u_long) vmaddrsp.xinu);
    kprintf("range	: 0x%08lx - 0x%08lx\n", (u_long) vmaddrsp.xinu,
	    (u_long) &etext);
    kprintf("    XINU DATA     (%d bytes)  \n",
	    (u_long) &edata - (u_long) &etext);
    kprintf("range	: 0x%08lx - 0x%08lx\n", (u_long) &etext,
	    (u_long) &edata);
    kprintf("    XINU BSS      (%d bytes)  \n",
	    (u_long) &end - (u_long) &edata);
    kprintf("range	: 0x%08lx - 0x%08lx\n", (u_long) &edata,
	    (u_long) &end);

    kprintf("    FRAME TABLES  (%d bytes)  \n",
	    (u_long)vmaddrsp.xheapmin  - (u_long) vmaddrsp.ftbl);
    kprintf("range	: 0x%08lx - 0x%08lx\n",(u_long)vmaddrsp.ftbl,
	    (u_long) vmaddrsp.xheapmin);

    kprintf("    XINU HEAP     (%d bytes) \n",
	    (u_long) vmaddrsp.xheapmax - (u_long) vmaddrsp.xheapmin);
    kprintf("range	: 0x%08lx - 0x%08lx\n", (u_long) vmaddrsp.xheapmin,
	    (u_long) vmaddrsp.xheapmax);

    kprintf("    DEVICE I/O SEGMENT \n");
    kprintf("range	: 0x%08x -  0x%08x\n", (u_long)IO_BASE_ADDR,
	   (u_long)IO_BASE_ADDR + SEGSIZ);

    kprintf("    DMA SPACE\n");
    kprintf("range	: 0x%08x -  0x%08x\n", (u_long)VM_DMA_BEGIN,
	    (u_long)VM_DMA_BEGIN + VM_DMA_NBYTES);
    kprintf("vm top	: 0x%08lx\n",(u_long) VM_MAXADDR);
    kprintf("\n");
#endif

#ifdef DEBUG
    kprintf("Initializing addr space table\n");
#endif	

    for (i=0; i<NADDR; i++) {	/* initialize address space tbl	*/
	addrtab[i].valid = FALSE;
	addrtab[i].tstamp = 1;
    }

#ifdef DEBUG
    kprintf("Initializing process table\n");
#endif	

    for (i=0 ; i<NPROC ; i++) {	/* initialize process table */
	proctab[i].pstate = PRFREE;
	proctab[i].tstamp = 1;
    }

    /* initialize free memory list */
    memlist.mnext = mptr = (struct mblock *) roundew(vmaddrsp.xheapmin);
    mptr->mnext = (struct mblock *)NULL;
    mptr->mlen = (unsigned) truncew ((unsigned)vmaddrsp.xheapmax -
				     (unsigned)vmaddrsp.xheapmin + sizeof(int) );
#ifdef DEBUG
    kprintf("Setting up free memory list\n");
    kprintf("    Heap: 0x%08x to 0x%08x (size = %d)\n", vmaddrsp.xheapmin,
	    vmaddrsp.xheapmax, mptr->mlen);
#endif

    for (i=0 ; i<NADDR ; i++) {	/* init heap space for all asid */
	vmemlist[i].mlen = 0;
	vmemlist[i].mnext = (struct mblock *) NULL;
    }

#ifdef DEBUG
    kprintf("Initializing xinu address space\n");
#endif	
    /* initialize xinu address space*/
    aptr = &addrtab[asidindex(XINUASID)];
    aptr->valid = TRUE;
    aptr->numthd = 1;
    aptr->thd_id = NULLPROC;
    aptr->psinformed = FALSE;

#ifdef DEBUG
    kprintf("Setting up the Null process (pid = %d)\n", NULLPROC);
#endif	
    /* initialize the null process */
    /* most of the null process PCB has already been initialized */
    pptr = &proctab[NULLPROC];
    pptr->pstate = PRCURR;
    strcpy(pptr->pname, "prnull");
    pptr->paddr = (int)nulluser;
    pptr->pargs = 0;
    pptr->pprio = 0;
    pptr->asid = XINUASID;
    pptr->next_thd = EMPTY;
    pptr->prev_thd = EMPTY;
    pptr->psinformed = FALSE;
    pptr->pnxtkin = BADPID;
    pptr->pdevs[0] = pptr->pdevs[1] = BADDEV;

    /* fill in the segment map for the null process */
    pptr->ctxt = GetContext();	/* get currect MMU context */
    ctxtbl[pptr->ctxt].used = TRUE;
    ctxtbl[pptr->ctxt].pid = NULLPROC;
    mru(&ctxtbl[pptr->ctxt]);	/* move context to most recent used position */

    /* somehow I have to switch the stack here, but at the moment I */
    /* really don't know the best way, so leave it run on the kern stk */
    /* It gets switched later after I return from this routine */

    currpid = NULLPROC;		/* make null process current	*/

    rdytail = 1 + (rdyhead=newqueue());/* initialize ready list */

#ifdef DEBUG
    kprintf("Calling mkinit\n");
#endif	

#ifdef	MEMMARK
    _mkinit();			/* initialize memory marking */
#endif

#ifdef DEBUG
    kprintf("Calling clkinit\n");
#endif	

#ifdef	RTCLOCK
    clkinit();			/* initialize r.t.clock	*/
#endif

#ifdef DEBUG
    kprintf("Calling netinit\n");
#endif	

    /* must init network buffers now for ethinit() to use */
    /* so this call must preceed the for() loop that initializes	*/
    /* the devices							*/
    netinit();	/* init network buffers */

#ifdef DEBUG
    kprintf("Starting to initialize devices\n");
#endif	

    for ( i=0 ; i<NDEVS ; i++ ) {		/* initialize devices	*/
	init(i);
    }

#ifdef DEBUG
    kprintf("Calling syscalinit()\n");
#endif	

    syscalinit();

#ifdef DEBUG
    kprintf("sysinit completed\n");
#endif	

    /* init the generic cache */
    cainit();

    return(OK);

}
