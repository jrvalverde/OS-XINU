/* traps.h -  Exception handling information */

/*---------------------------------------------------------------------------
 * Since the SPARC does not define an exception stack frame, we are free
 * to define our own.  Below is the structure of the exception frame created
 * by genericint().  Genericint() passes a pointer to this structure to the
 * interrupt handling routines.  Note, we are restricted to SPARCMINFRAME bytes
 *---------------------------------------------------------------------------
 */

#ifndef ASM

struct exception_frame {
	unsigned int	vector;		/* interrupt vector number	*/
	char		*pc;		/* pc at time of interrupt	*/
	char		*npc;		/* npc at time of interrupt	*/
	unsigned int	psr;		/* psr immediately after intrpt	*/
	unsigned int	cwp;		/* intrpt window - same as in psr */
	unsigned int	wim;		/* wim at time of interrupt	*/
	char		*sp_prev_win;	/* sp of win before intrp window */
	char		*sp_next_win;	/* sp of win after intrp window */
};

#endif

/* Hardware defined trap vectors */
#define H_TRAP_RESET			0
#define H_TRAP_BAD_INST_ACCESS		1
#define H_TRAP_ILLEGAL_INST		2
#define H_TRAP_PRIV_INST_ACCESS		3
#define H_TRAP_FP_DISABLED		4
#define H_TRAP_CP_DISABLED		36
#define H_TRAP_WIN_OVERFLOW		5
#define H_TRAP_WIN_UNDERFLOW		6
#define H_TRAP_ADDR_NOT_ALIGNED		7
#define H_TRAP_FP_EXCEPTION		8
#define H_TRAP_CP_EXCEPTION		40
#define H_TRAP_BAD_DATA_ACCESS		9
#define H_TRAP_TAG_OVERFLOW		10
#define H_TRAP_INTR_LVL_1		17
#define H_TRAP_INTR_LVL_2		18
#define H_TRAP_INTR_LVL_3		19
#define H_TRAP_INTR_LVL_4		20
#define H_TRAP_INTR_LVL_5		21
#define H_TRAP_INTR_LVL_6		22
#define H_TRAP_INTR_LVL_7		23
#define H_TRAP_INTR_LVL_8		24
#define H_TRAP_INTR_LVL_9		25
#define H_TRAP_INTR_LVL_10		26
#define H_TRAP_INTR_LVL_11		27
#define H_TRAP_INTR_LVL_12		28
#define H_TRAP_INTR_LVL_13		29
#define H_TRAP_INTR_LVL_14		30
#define H_TRAP_INTR_LVL_15		31

