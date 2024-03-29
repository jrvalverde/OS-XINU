/*      @(#)a.out.h 1.7 89/09/06 SMI; from UCB 4.1 83/05/03       */

#ifndef _sparc_a_out_h
#define _sparc_a_out_h

/*	@(#)exec.h 1.7 88/08/19 SMI	*/

/*
 * Copyright (c) 1985 by Sun Microsystems, Inc.
 */

#ifndef _sys_exec_h
#define _sys_exec_h

/*
 * format of the exec header
 * known by kernel and by user programs
 */
struct exec {
#ifdef sun
	unsigned char	a_dynamic:1;	/* has a __DYNAMIC */
	unsigned char	a_toolversion:7;/* version of toolset used to create this file */
	unsigned char	a_machtype;	/* machine type */
	unsigned short	a_magic;	/* magic number */
#else
	unsigned long	a_magic;	/* magic number */
#endif
	unsigned long	a_text;		/* size of text segment */
	unsigned long	a_data;		/* size of initialized data */
	unsigned long	a_bss;		/* size of uninitialized data */
	unsigned long	a_syms;		/* size of symbol table */
	unsigned long	a_entry;	/* entry point */
	unsigned long	a_trsize;	/* size of text relocation */
	unsigned long	a_drsize;	/* size of data relocation */
};

#define	OMAGIC	0407		/* old impure format */
#define	NMAGIC	0410		/* read-only text */
#define	ZMAGIC	0413		/* demand load format */

/* machine types */

#ifdef sun
#define M_OLDSUN2	0	/* old sun-2 executable files */
#define M_68010		1	/* runs on either 68010 or 68020 */
#define M_68020		2	/* runs only on 68020 */
#define M_SPARC		3	/* runs only on SPARC */

#define TV_SUN2_SUN3	0
#define TV_SUN4		1
#endif sun

#endif /*!_sys_exec_h*/

/*
 * memory management parameters
 */

#define AOUT_PAGSIZ	0x02000		/* changed by griff to avoid	*/
#define AOUT_SEGSIZ	AOUT_PAGSIZ	/* conflicting with vmem.h	*/
#define	OLD_PAGSIZ	0x00800	/*  Page   size under Release 2.0 */
#define	OLD_SEGSIZ	0x08000	/* Segment size under Release 2.0 */

/*
 * returns 1 if an object file type is invalid, i.e., if the other macros
 * defined below will not yield the correct offsets.  Note that a file may
 * have N_BADMAG(x) = 0 and may be fully linked, but still may not be
 * executable.
 */

#define	N_BADMAG(x) \
	((x).a_magic!=OMAGIC && (x).a_magic!=NMAGIC && (x).a_magic!=ZMAGIC)

/*
 * relocation parameters. These are architecture-dependent
 * and can be deduced from the machine type.  They are used
 * to calculate offsets of segments within the object file;
 * See N_TXTOFF(x), etc. below.
 */

#define	N_PAGSIZ(x) \
	((x).a_machtype == M_OLDSUN2? OLD_PAGSIZ : AOUT_PAGSIZ)
#define	N_SEGSIZ(x) \
	((x).a_machtype == M_OLDSUN2? OLD_SEGSIZ : AOUT_SEGSIZ)

/*
 * offsets of various sections of an object file.
 */

#define	N_TXTOFF(x) \
	/* text segment */ \
	( (x).a_machtype == M_OLDSUN2 \
	? ((x).a_magic==ZMAGIC ? N_PAGSIZ(x) : sizeof (struct exec)) \
	: ((x).a_magic==ZMAGIC ? 0 : sizeof (struct exec)) )

#define	N_DATOFF(x)   /* data segment */	\
	(N_TXTOFF(x) + (x).a_text)

#define	N_TRELOFF(x)  /* text reloc'n */	\
	(N_DATOFF(x) + (x).a_data)

#define	N_DRELOFF(x) /* data relocation*/	\
	(N_TRELOFF(x) + (x).a_trsize)

#define	N_SYMOFF(x) \
	/* symbol table */ \
	(N_TXTOFF(x)+(x).a_text+(x).a_data+(x).a_trsize+(x).a_drsize)

#define	N_STROFF(x) \
	/* string table */ \
	(N_SYMOFF(x) + (x).a_syms)

/*
 * Macros which take exec structures as arguments and tell where the
 * various pieces will be loaded.
 */

#define	_N_BASEADDR(x) \
	(((x).a_magic == ZMAGIC) && ((x).a_entry < N_PAGSIZ(x)) ? \
	    0 : N_PAGSIZ(x))

#define N_TXTADDR(x) \
	((x).a_machtype == M_OLDSUN2 ? N_SEGSIZ(x) : _N_BASEADDR(x))

#define N_DATADDR(x) \
	(((x).a_magic==OMAGIC)? (N_TXTADDR(x)+(x).a_text) \
	: (N_SEGSIZ(x)+((N_TXTADDR(x)+(x).a_text-1) & ~(N_SEGSIZ(x)-1))))

#define N_BSSADDR(x)  (N_DATADDR(x)+(x).a_data)

/* sizes of the text, data, and bss segments (in mem AND the file) */
#define N_TEXTSIZE(exec)        (exec).a_text
#define N_DATASIZE(exec)        (exec).a_data
#define N_BSSSIZE(exec)         (exec).a_bss

/*
 * Format of a relocation datum.
 */

/*
 * Sparc relocation types
 */
 
enum reloc_type
{
	RELOC_8,	RELOC_16,	RELOC_32,	/* simplest relocs    */
	RELOC_DISP8,	RELOC_DISP16,	RELOC_DISP32,	/* Disp's (pc-rel)    */
	RELOC_WDISP30,	RELOC_WDISP22,			/* SR word disp's     */
	RELOC_HI22,	RELOC_22,			/* SR 22-bit relocs   */
	RELOC_13,	RELOC_LO10,			/* SR 13&10-bit relocs*/
	RELOC_SFA_BASE,	RELOC_SFA_OFF13,		/* SR S.F.A. relocs   */
	RELOC_BASE10,	RELOC_BASE13,	RELOC_BASE22,	/* base_relative pic */
	RELOC_PC10,	RELOC_PC22,			/* special pc-rel pic*/
	RELOC_JMP_TBL,					/* jmp_tbl_rel in pic */
	RELOC_SEGOFF16,					/* ShLib offset-in-seg*/
	RELOC_GLOB_DAT, RELOC_JMP_SLOT, RELOC_RELATIVE, /* rtld relocs        */
};

/*
 * Format of a relocation datum.
 */
 
struct reloc_info_sparc	/* used when header.a_machtype == M_SPARC */
{
	unsigned long int r_address;	/* relocation addr (offset in segment)*/
	unsigned int	r_index   :24;	/* segment index or symbol index      */
	unsigned int	r_extern  : 1;	/* if F, r_index==SEG#; if T, SYM idx */
	int			  : 2;	/* <unused>			      */
	enum reloc_type r_type    : 5;	/* type of relocation to perform      */
	long int	r_addend;	/* addend for relocation value	      */
};



/*
 * Format of a symbol table entry
 */
struct	nlist {
	union {
		char	*n_name;	/* for use when in-core */
		long	n_strx;		/* index into file string table */
	} n_un;
	unsigned char	n_type;		/* type flag (N_TEXT,..)  */
	char	n_other;		/* unused */
	short	n_desc;			/* see <stab.h> */
	unsigned long	n_value;	/* value of symbol (or sdb offset) */
};

/*
 * Simple values for n_type.
 */
#define	N_UNDF	0x0		/* undefined */
#define	N_ABS	0x2		/* absolute */
#define	N_TEXT	0x4		/* text */
#define	N_DATA	0x6		/* data */
#define	N_BSS	0x8		/* bss */
#define	N_COMM	0x12		/* common (internal to ld) */
#define	N_FN	0x1e		/* file name symbol */

#define	N_EXT	01		/* external bit, or'ed in */
#define	N_TYPE	0x1e		/* mask for all the type bits */

/*
 * Dbx entries have some of the N_STAB bits set.
 * These are given in <stab.h>
 */
#define	N_STAB	0xe0		/* if any of these bits set, a dbx symbol */

/*
 * Format for namelist values.
 */
#define	N_FORMAT	"%08x"

/*
 * secondary sections.
 * this stuff follows the string table.
 * not even its presence or absence is noted in the
 * exec header (?). the secondary header gives
 * the number of sections. following it is an
 * array of "extra_nsects" int's which give the
 * sizeof of the individual sections. the presence of
 * even the header is optional.
 */
 
#define EXTRA_MAGIC     1040            /* taxing concept  */
#define EXTRA_IDENT     0               /* ident's in 0th extra section */

struct extra_sections {
	   	int     extra_magic;            /* should be EXTRA_MAGIC */
	   	int     extra_nsects;           /* number of extra sections */
};

#endif /*!_sparc_a_out_h*/
