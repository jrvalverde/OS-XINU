/* vmem.h - virtual memory constants and structures */

/* Machine Dependent VM constants and data structures */

#include <vmem.sparc.h>			/* SPARC specific stuff		*/

/* Machine Independent VM constants and data structures */

#define  GCOPYPG	0		/* general purpose copy page	*/
#define  PGSCOPY	1		/* paging sender copy page	*/
#define  PGRCOPY	2		/* paging receiver copy page	*/
#define  MEMCOPY	3		/* memory copy page		*/

#ifndef ASM

/* vm information structures */
struct pginfo {				/* paging related info		*/
    long diskfaults;			/* # page faults - pg from disk	*/
    long simfaults;			/* # page faults - simulate ref bit*/
    long memfaults;			/* # page faults - pg from mem	*/
    long zodfaults;			/* # page faults - zero-on-demand*/
    long pagingouts;			/* # paging out times		*/
    long addrviolation;			/* # address violations		*/
};

struct vmaddrsp {			/* vm address space layout	*/
    char *minaddr;			/* min virtual address 		*/
    char *maxheap;			/* max heap address		*/
    char *minstk;			/* min stack address		*/
    char *stkstrt;			/* start of all stacks		*/
    char *kernstk;			/* start of kernal stack	*/
    char *rsaio;			/* max RSA I/O address		*/
    char *maxsp;			/* max S/P address		*/
    char *xinu;				/* start (min) xinu tdb addr	*/
    char *devio;			/* start (min) device I/O addr	*/
    char *pgtbl;			/* start of page tables		*/
    char *ftbl;				/* start of frame table		*/
    char *xheapmin;			/* min xinu heap address	*/
    char *xheapmax;			/* max xinu heap address	*/
    char *copyaddr;			/* ptr to sys space copy page	*/
    char *pgscpaddr;			/* paging sdr copy page		*/
    char *pgrcpaddr;			/* paging rcvr copy page	*/
    struct ppte	 copypte;		/* pte used for pys pg access	*/
    struct ppte	 pgscopypte;		/* pte used for pg sender access*/
    struct ppte	 pgrcopypte;		/* pte used for pg rcvr  access*/
    int	 copysize;			/* size of the copy area	*/
};

#define MAX_MEM_BANKS   4               /* 4 should cover most machines */

/* variables */
extern struct pginfo pg;		/* page information structure	*/
extern struct vmaddrsp vmaddrsp;	/* vm address space info	*/
extern struct mblock vmemlist[];	/* free list in p0 for each proc */
extern struct physmemdesc physmembanks[];/* description of phys. mem. banks*/
extern unsigned mem_size;		/* size of real (physical) memory*/
extern int xkernsize;			/* size of xinu-txt,data,bss,heap*/

#endif ASM


/* virtual memory utility routines */
/* round vmaddress up to next pg */
#define roundpg(x)	((((unsigned) x)+(PGSIZ-1)) & ~((unsigned)(PGSIZ-1)))
/* truncate vmaddress to cur pg	*/
#define	truncpg(x)	(((unsigned) x) & ~((unsigned)(PGSIZ-1)))
/* return vm address of page x	*/
#define	tovaddr(x)	(((unsigned) x) << LOG2_PGSIZ)
/* return page of vm address x	*/
#define	topgnum(x)	(((unsigned) x) >> LOG2_PGSIZ)
/* return offset within page	*/
#define tooffset(x)	(((unsigned) x) % PGSIZ)

#define NFRAMES(nbytes)	(roundpg((nbytes)) >> LOG2_PGSIZ) 

/****************************************************************/
/*                                                              */
/*                   PAGING INFORMATION                         */
/*								*/
/* NOTE: MUST choose either one to do paging.			*/
/*								*/
/*       We recommend using NFS pagging.			*/
/*                                                              */
/****************************************************************/
#define NFS_PAGING		/* use NFS paging               */
/*#define XPP_PAGING		/* use remote memory server     */

#if !defined(NFS_PAGING) && !defined(XPP_PAGING)
FATAL: must define either "XPP_PAGING" or "NFS_PAGING"
#endif
