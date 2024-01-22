/* vmem.sparc.h -  constants for VM Xinu on a SPARC architecture */

#ifndef ASM
extern int mmu_nctx;		/* the number of MMU context 		*/
extern int mmu_npmeg;		/* the number MMU PMEGs			*/
extern int vac_line_size;	/* Virtual Address Cache line size	*/
extern int vac_size;		/* VAC size				*/
#endif

/* The following MMU constants vary from model to model */

#if defined(SPARC_ELC) || defined(SPARC_SLC)
#define VM_NUM_CONTEXTS		8	/* number of contexts in mmu	*/
#define VM_NUM_SEGMENTS		4096	/* number of segs per context	*/
#define	VM_PGS_PER_SEG		64	/* number of pages per segment	*/
#define VM_NUM_PMEGS		128	/* number of PMEGs in mmu. 	*/

#define LOG2_VM_NUM_CONTEXTS	3	/* log2 num of contexts in mmu	*/
#define LOG2_VM_NUM_SEGMENTS	12	/* log2 num of segs per context	*/
#define	LOG2_VM_PGS_PER_SEG	6	/* log2 num of pages per segment*/
#define LOG2_VM_NUM_PMEGS	8	/* log2 num of PMEGs in mmu	*/
#endif

#if defined(SPARC_SLC) || defined (SPARC_1) || defined(SPARC_ELC)
#define	VAC_SIZE		0x10000	/* virtual address cache size	*/
#define	VAC_LINESIZE		0x10	/* virt address cache linesize */
#endif

#define USE_THE_VAC		/* uncomment this line to turn the VAC on */

#define PGSIZ			0x1000	/* size of a single page	*/
#define SEGSIZ			0x40000	/* (VM_PGS_PER_SEG * PGSIZ)	*/
					/* size of a segment		*/
#define LOG2_PGSIZ		12	/* log2 of the size of a page	*/


/************************************************************************/
/************************************************************************/
/*									*/
/*			Configuration Constants				*/
/*									*/
/* The following constants indicate the starting address of the kernel	*/
/* in virtual memory and also the sizes of various virtual memory	*/
/* regions.								*/
/************************************************************************/
/************************************************************************/

/* Kernel Starting Location in Virtual Memory */
#define VM_KERNEL_OFFSET	0x0fd00000

/* Sizes of various virtual memory regions */

/* Total kernel size - text, data, bss, and heap */
#define VM_KERNEL_SIZE		0x00200000	/* 2 MEG 		*/

/* The Size of the Shared/Private Area */
#define VM_SP_SIZE		0x00000000 /* none at the moment	*/

/* The Size of a User's RSA I/O area */
#define VM_RSA_SIZE		0x00001000 /* 4k = 1 page		*/

/* The Size of a User's Kernel Stack */
#define VM_KSTK_SIZE		0x00002000 /* 8k = 2 pages		*/

/* Max Size of a User's Stack */
#define VM_USER_STACK_SIZE	0x100000	/* User Stack Size  1M	*/

/* Max Size of a User's Area - includes user text, data, bss, and heap	*/
#define VM_USER_HEAP_SIZE	0x1000000	/* User Heap Size  16M	*/

/* Address of device control registers - on some architectures, the	*/
/* hardware defines the location in vm of the device control registers.	*/
/* On the SPARC, we are free to put them anywhere that we like in the	*/
/* Virtual address space.  The addresses chosen below are psuedo-random	*/
/* addresses (i.e., we did try to find locations that are out of the	*/
/* way but still in the same segment if possible.			*/
/* NOTE: These must jive with the addresses in the Configuration file!	*/
/* These addresses are mapped in in the map_devs() function 		*/

/* The base address of the I/O seg - assuming they all fit in one segment*/
/* Each device uses one page of this OB_IO segement. 			*/
#define IO_BASE_ADDR		(0xFFD00000)

/* General Interrupt Control Register */
#define INTERRUPT_REG		(0xFFD0A000)

/* Clock Control Register */
#define	CLOCK_REG		(0xFFD06000)

/* EEPROM Location */
#define EEPROM_ADDR		(0xFFD04000)

/* Serial Line Unit Control Register */
#define	SLU_REG			(0xFFD02000)

/* Define this if you have a Lance ethernet chip - SPARC does */
#define AMD_LANCE_CHIP

/* Lance Ethernet Control Register */
#ifdef AMD_LANCE_CHIP
#define ETHERNET_REG		(0xFFD10000)
#else
#define ETHERNET_REG		(0xFFD0C000)
#endif

/* SCSI Control Register */
#define	SCSI_REG		(0xFFD12000)

/* The good place to do DMA to */
#define A_GOOD_DMA_STARTING_ADDR 0xfff40000

/* Before passing a dma address to a dma device (like the LANCE chip), 	*/
/* you must mask off the following high order bits.			*/
#define DMA_MASK		0x00ffffff

/************************************************************************/
/*		End of Kernel Configuration Constants			*/
/************************************************************************/


/************************************************************************/
/*									*/
/*		How Virtual Memory is laid out				*/
/*									*/
/************************************************************************/

/* Where the User T,D,Bss,Heap go */
#define VM_MINADDR		(PGSIZ)
#define VM_USER_HEAP_END	(VM_USER_HEAP_SIZE-1)


/* Where the Xinu text begins in VM */
#define VM_KERNEL_TEXT		(VM_KERNEL_OFFSET + 0x4000)
#define VM_KERNEL_MAXADDR	(VM_KERNEL_OFFSET+VM_KERNEL_SIZE-1)
#define VM_KERNEL_NUMSEGS	(VM_KERNEL_SIZE/SEGSIZ)
#define VM_MAXADDR		((VM_NUM_SEGMENTS*SEGSIZ) - 1)

/* Now we define some special regions (e.g., I/O dma regions, etc.)	*/

/* Where the Device I/O space resides */
#define VM_DEVIO		(IO_BASE_ADDR)

/* VM set aside for DVMA buffers */
#define VM_DMA_BEGIN		A_GOOD_DMA_STARTING_ADDR
#define VM_DMA_NUMSEGS		1
#define VM_DMA_NBYTES		VM_DMA_NUMSEGS * SEGSIZ

/* VM pages set aside for Copying */
#define VM_COPY_BEGIN		0x0ff40000
#define VM_COPY_END		((VM_COPY_BEGIN+PGSIZ) - 1)
#define PGS_COPY_BEGIN		(VM_COPY_BEGIN+PGSIZ)
#define PGS_COPY_END		((PGS_COPY_BEGIN+PGSIZ) - 1)
#define PGR_COPY_BEGIN		(PGS_COPY_BEGIN+PGSIZ)
#define PGR_COPY_END		((PGR_COPY_BEGIN+PGSIZ) - 1)

/* Virtual Memory reserved for copying pages, page servers, etc */
#define VM_RESPG_BEGIN         	0x0ff45000	
#define VM_RESPG_END            0x0ff4ffff
#define VM_RESPG_NPAGES         topgnum((VM_RESPG_END - VM_RESPG_BEGIN) + 1)

/* Segment set aside for mapping pte's; Note: this value must be seg aligned */
#define VM_MAPPING_SEGMENT	0x0ff80000

/************************************************************************/
/*		End of how virtual memory is laid out			*/
/************************************************************************/

/*
 * Address space identifiers.
 */
#define ASI_UP  0x8             /* user program */
#define ASI_SP  0x9             /* supervisor program */
#define ASI_UD  0xA             /* user data */
#define ASI_SD  0xB             /* supervisor data */
#define ASI_FCS 0xC             /* flush cache segment */
#define ASI_FCP 0xD             /* flush cache page */
#define ASI_FCC 0xE             /* flush cache context */
#define ASI_CTL 0x2             /* control space*/
#define ASI_SM  0x3             /* segment map */
#define ASI_PM  0x4             /* page map */

/*
 * ASI_CTL addresses
 */
#define	CONTEXT_REG		0x30000000
#define	SYSTEM_ENABLE		0x40000000
#define	SYNC_ERROR_REG		0x60000000
#define	SYNC_VA_REG		0x60000004
#define	ASYNC_ERROR_REG		0x60000008
#define	ASYNC_VA_REG		0x6000000C
#define	CACHE_TAGS		0x80000000
#define	CACHE_DATA		0x90000000
#define	UART_BYPASS		0xF0000000

/* Some handy constants based on the above */
#define MAXADDRPMEGS	((VM_USER_HEAP_END/SEGSIZ)+1)
					/* max # pmegs saved in addrtab	*/
#define MAXPROCPMEGS	(((VM_SP_SIZE+VM_RSA_SIZE+VM_KSTK_SIZE		\
			  +VM_USER_STACK_SIZE)/SEGSIZ)+1)
					/* max # pmegs saved in proctab	*/

#ifndef ASM

/* special PMEG numbers */
#define VM_PMEG_INVALID	(VM_NUM_PMEGS-1)
				/* PMEG with all invlaid pages		*/
/*#define VM_PMEG_NONE	VM_NUM_PMEGS + 1*/
				/* Used in proc and addr table to	 */
				/* indicate that the pmeg was reclaimed	*/
				/* and moved out of the mmu into the	*/
				/* the proc or addr space table		*/


/* SPARC Page Fault Types: magic values found in the SunOS 4.1 buserr.h file */
#define vm_translation_error(err)	( ((unsigned int)(err)) & 0x00000080 )
#define vm_protection_error(err)	( ((unsigned int)(err)) & 0x00000040 )
#define vm_timeout_error(err)		( ((unsigned int)(err)) & 0x00000020 )
#define vm_sbus_error(err)		( ((unsigned int)(err)) & 0x00000010 )
#define vm_sync_mem_error(err)		( ((unsigned int)(err)) & 0x00000008 )
#define vm_addr_size_error(err)		( ((unsigned int)(err)) & 0x00000002 )
#define vm_watchdog_reset_error(err)	( ((unsigned int)(err)) & 0x00000001 )
#define vm_write_access(err)		( ((unsigned int)(err)) & 0x00008000 )
#define vm_read_access(err)		( ! vm_write_access(err) )

struct pfinfo {
    unsigned int pc;		/* program counter at time of fault	*/
    unsigned int npc;		/* program counter at time of fault	*/
    unsigned int vaddr;		/* address that caused the fault	*/
    unsigned int reason;	/* page fault type (reason)		*/
    unsigned int on_data_access;/* data access (1), else instr access (0) */
    unsigned int on_write_access;/* write access (1), else read access (0) */
};


/*  Definition of frame and page table entries */

struct mmupte {				/* index into the mmu for pte	*/
    int		pmeg;			/* pmeg number			*/
    int		ptenum;			/* ptenum within pmeg (0-63)	*/
};

struct ppte {				/* mach dependent pte pointer	*/
    Bool	inmmu;			/* 1: in mmu, 0: in memory only	*/
    struct {
	union pte	*pte;		/* ptr to pte in memory		*/
	struct mmupte	mmuindex;	/* pmeg # and pte within pmeg	*/
    } loc;
};


union pte {				/* definition of page tbl entry	*/
    long value;				/* value as a whole, 32 bits    */
    struct {
	unsigned hvalid     : 1;	/* Valid,      1 = Valid page   */

	unsigned prot       : 2;	/* protection:			*/
       					/* 00 - nowrite, user   	*/
       					/* 01 - nowrite, super		*/
       					/* 10 - write,   user		*/
       					/* 11 - write,	 super		*/

	unsigned dontcache  : 1;	/* Don't Cache, 1 = Don't cache	*/
	unsigned type       : 2;	/* Type:       00, 01, 10, 11   */
	unsigned ref        : 1;	/* Accessed,   1 = been accessed*/
	unsigned mod        : 1;	/* Modified,   1 = been modified*/

	/* The next 3 are NOT hardware bits */
	/* The sun4c mmu does not allow us to change these bits, so the	*/
	/* value of these bits will always be found in the in memory copy */
	unsigned inmem      : 1;	/* Page in memory?		*/
	unsigned tvalid     : 1;	/* 1: within valid memory regions*/
	/* the following bit has meaning only when inmem = 0 	*/
	unsigned zero	    : 1;	/* 1: Zero on demand		*/
	unsigned aout	    : 1;	/* 1: load from a.out		*/
	unsigned unused     : 4;	/* unused			*/
	
	unsigned pfn        : 16;	/* Frame number			*/
    } st;				/* normal usage of pte		*/
};

#define MMUPTEBITS	0xff0007ff	/* only bits setable in mmu	*/
#define XINUPTEBITS	0x00f80000	/* bits in pte used by xinu	*/
#define mmuptebits(val)	 ((val)&MMUPTEBITS) /* extract mmu pte bits	*/
#define xinuptebits(val) ((val)&XINUPTEBITS) /* extract xinu pte bits	*/

struct savepmeg {			/* saved memory copy of mmu pte */
    int		pmeg;			/* Pmeg which maps this segment	*/
				    	/*   127 - invalid		*/
					/*   128 - pmeg reclaimed, last	*/
					/*         MMU info is here	*/
    long	savpte[VM_PGS_PER_SEG];
					/* Saved contents of MMU if	*/
					/* pmeg = 128.  Invalid if 	*/
					/* pmeg = 127.  Otherwise Mod,	*/
					/* Ref, Hvalid bits are in the	*/
					/* mmu in pmeg, and xinu defined*/
					/* bits like inmem, tvalid, and	*/
					/* list are found here in mem	*/
};

struct procpgtbl {
	/* saved copy of page table stuff for stack, etc */
	struct	savepmeg redpmeg[MAXPROCPMEGS];	/* contents of MMU pmegs*/
};

struct addrpgtbl {
	struct	savepmeg redpmeg[MAXADDRPMEGS];	/* contents of MMU pmegs*/
};

/* in memory copies of page table entries */
extern  struct savepmeg sysredpmeg[];	/* redundant copy of sys pg tbl */
extern	struct procpgtbl procpmegs[];	/* redundant copy of process pgtbl */
					/* one for each process		 */
extern	struct addrpgtbl addrpmegs[];	/* redundant copy of addr sp pgtbl */
					/* one for each addr sp		*/
extern struct savepmeg copypmeg;	/* redundant copy of copy pgtbl	*/
extern int curcontext;			/* current context (cur seg tbl) */


/* PMEG info */
#define VM_PMEG_LOCKED 01
#define VM_PMEG_FREE   02
#define VM_PMEG_INUSE  03

struct pmege {
    struct pmege *next;			/* next pmeg			*/
    struct pmege *prev;			/* prev pmeg			*/
    int		num;			/* number of this pmeg		*/
    int		stat;			/* free, inuse, locked, etc	*/
    int		pid;			/* process which owns this pmeg	*/
    unsigned long vaddr;		/* virtual address which it maps*/
};

extern struct pmege pmegtbl[];

#define VM_PMEG_LISTHD	VM_NUM_PMEGS	 /* pmeg list head		*/
#define VM_PMEG_LISTTL	VM_NUM_PMEGS + 1 /* pmeg list tail		*/

/* pmeg free list */
#define VM_PMEG_FREEHD	VM_NUM_PMEGS + 2 /* pmeg free list head		*/
#define VM_PMEG_FREETL	VM_NUM_PMEGS + 3 /* pmeg free list tail		*/

/************************************************************************/
/*									*/
/*		FOR TALKING TO THE HARDWARE				*/
/*									*/
/************************************************************************/

/* Protection bits */
#define PAGE_WRITE	0x4
#define SUPER_ONLY	0x2
#define	DONT_CACHE	0x1
#define DO_CACHE	0x0

/* Page types */
#define ONBOARD_MEM	0	/* "Onboard" (P2 bus) */
#define ONBOARD_IO	1	/* Onboard devices */
#define VME_16		2	/* VMEbus 16 bit data space */
#define VME_32		3	/* VMEbus 32 bit data space */

/* Structure of a virtual address */
typedef union
  {
    /* Fields */
    struct
      {
	unsigned long	unused:2;
	unsigned long	segment:12;	/* Segment number */
	unsigned long	page:6;		/* Page number within segment */
	unsigned long	byte:12;	/* Byte number */
      }
    f;
    char *p;		/* Whole thing as a character pointer */
    unsigned long u;	/* Whole thing as an unsigned 32-bit quantity */
  }
sunvaddr;

/* Break down a Virtual Address - see sunvaddr above */
#define VMSegOf(a)  (((unsigned long)(a) >> 18) & 0x00000fff)
#define VMPageOf(a) (((unsigned long)(a) >> 12) & 0x0000003f)
#define VMByteOf(a) (((unsigned long)(a)      ) & 0x00000fff)

/* Round an address up to the nearest page boundary */
#define uptopage(addr) \
	(((((unsigned long) (addr)) - 1) & (~(PAGESIZ-1))) + PAGESIZ)

/* Round an address up to the nearest segment boundary */
#define uptoseg(addr) \
  	(((((unsigned long) (addr)) - 1) & (~(SEGSIZ-1))) + SEGSIZ)

/* Round an address down to the nearest page boundary */
#define downtopage(addr) 	(((unsigned long) (addr)) & ~(PGSIZ-1))

/* Round an address down to the nearest segment boundary */
#define downtoseg(addr) 	(((unsigned long) (addr)) & ~(SEGSIZ-1)) 

#define STK_ALIGN_8(x)		(((unsigned long)(x)) & ~(7))

/* Read and write MMU registers */
#define SetPageMap(addr, value)		set_pmeg(addr, value)
#define SetSegMap(addr, value)		set_smeg(addr, value)
#define GetPageMap(addr)		get_pmeg(addr)
#define GetSegMap(addr)			get_smeg(addr)
#define SetContext(cxt)			set_context(cxt)
#define GetContext()			get_context()


/* Routines for manipulating page table entries */
/* Fun with MACROS!!!! */

extern union pte pteTemp;

#define PTEGET(ppte,field)  (((ppte)->inmmu==TRUE)?			\
    (SetSegMap(VM_MAPPING_SEGMENT,(ppte)->loc.mmuindex.pmeg),		\
     pteTemp.value=GetPageMap(VM_MAPPING_SEGMENT			\
			  +(PGSIZ*(ppte)->loc.mmuindex.ptenum)),	\
     pteTemp.field)							\
    :									\
    ((ppte)->loc.pte->field))						

#define PTESET(ppte,field,newvalue) 					\
    if ((ppte)->inmmu == TRUE) {					\
        union pte pte; unsigned long vaddr;				\
	SetSegMap(VM_MAPPING_SEGMENT,(ppte)->loc.mmuindex.pmeg);	\
	vaddr = VM_MAPPING_SEGMENT + PGSIZ*(ppte)->loc.mmuindex.ptenum;	\
        pte.value = GetPageMap(vaddr);					\
	pte.field = newvalue;						\
        SetPageMap(vaddr, pte.value);					\
    }									\
    else {								\
	(ppte)->loc.pte->field = (newvalue);				\
}



/* routines for reading page table entry values */
#define getvalue(ptr)		(mmuptebits(PTEGET(ptr,value)) |	\
				 xinuptebits((ptr)->loc.pte->value))
#define getpfn(ptr)		PTEGET(ptr,st.pfn)
#define getmod(ptr)		PTEGET(ptr,st.mod)
#define getprot(ptr)		PTEGET(ptr,st.prot)
#define gethvalid(ptr)		PTEGET(ptr,st.hvalid)
#define getreference(ptr)	PTEGET(ptr,st.ref)
#define getdontcache(pte)	PTEGET(ptr,st.dontcache)
#define gettype(pte)		PTEGET(ptr,st.type)
/* the following bits are not MMU hardware bits */
#define getzero(ptr)		((ptr)->loc.pte->st.zero)
#define getaout(ptr)		((ptr)->loc.pte->st.aout)
#define getinmem(ptr)		((ptr)->loc.pte->st.inmem)
#define gettvalid(ptr)		((ptr)->loc.pte->st.tvalid)



#define setvalue(ptr, val)	PTESET((ptr),value,val);	\
				((ptr)->loc.pte->value = val)
#define setpfn(ptr, val)	PTESET((ptr),st.pfn,val)
#define setmod(ptr, val)	PTESET((ptr),st.mod,val)
#define setprot(ptr, val)	PTESET((ptr),st.prot,val)
#define sethvalid(ptr, val)	PTESET((ptr),st.hvalid,val)
#define setreference(ptr, val)	PTESET((ptr),st.ref,val)
#define setdontcache(ptr,val)	PTESET((ptr),st.dontcache,val)
#define settype(ptr,val)	PTESET((ptr),st.type,val)
/* the following bits are not MMU hardware bits */
#define setinmem(ptr, val)	((ptr)->loc.pte->st.inmem = val)
#define settvalid(ptr, val)	((ptr)->loc.pte->st.tvalid = val)
#define setzero(ptr, val)	((ptr)->loc.pte->st.zero = val)
#define setaout(ptr, val)	((ptr)->loc.pte->st.aout = val)

/* SPARC only has 2 protection bits => 4 protection types */
#define	PG_RU	(0x0)			/* read/user mode		*/
					/* 	- user read only	*/
					/* 	- supervisor read only	*/
#define PG_RS	(0x1)			/* read/supervisor mode		*/
					/* 	- supervisor read only	*/
#define	PG_WU	(0x2)			/* write/user mode		*/
					/* 	- user read/write	*/
					/* 	- supervisor read/write	*/
#define PG_WS	(0x3)			/* write/supervisor mode	*/
					/* 	- supervisor read/write	*/

/* define page protection codes - as best as possible */
#define U_K	PG_RS			/* no access - or close to it	*/
#define U_KR	PG_RS			/* user none, kernel r		*/
#define U_KRW	PG_WS			/* user none, kernel rw		*/
#define UR_KR	PG_RU			/* user r, kernel r		*/
#define URW_KRW	PG_WU			/* user rw, kernel rw		*/

/* initialization bitmask for page table entries */
/* these may need a second look - but I think they are right */
#define PTEZEROMASK	0x40600000	/* all rw, do cache, tvalid, zero_on_demand */
#define PTE_LOAD_AOUT	0x40500000	/* all rw, do cache, tvalid, aout */

/* routines for mapping addresses to saved pmegs in proctab */
#define FIRSTTHREADSEG	VMSegOf((unsigned)vmaddrsp.minstk)
#define procsegindex(address)	(VMSegOf(address) - FIRSTTHREADSEG)


/* context definitions for managing mmu contexts */
#define	CTXTNUM		((VM_NUM_CONTEXTS)+2) /* num MMU contexts plus 2 */
#define CTXTHD		CTXTNUM - 2	/* context queue head,  its "next" is
					   least recently used context      */
#define CTXTTL		CTXTNUM - 1     /* context queue tail,  its "prev" is
 					   most recently used context	    */
struct ctxte {				/* context information		*/
	int	used;
	int	pid;
	int	num;
	struct  ctxte *nextc;
	struct  ctxte *prevc;
} ctxte;

extern struct	ctxte	ctxtbl[];	/* for mamaging context in MMU	*/
extern int savesp;
#endif ASM
