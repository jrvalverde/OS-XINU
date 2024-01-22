/* user_traps.h - definition of Ticc Trap Vectors (i.e., vectors 128 - 256) */

/* Define the offset in the SPARC trap table at which user traps begin */
#define TICC_OFFSET		128


/* Define Ticc instruction traps */

#define	U_TRAP_DIV_BY_ZERO	0x2	/* divide by zero trap		*/
#define	U_TRAP_SYSCALL		0x4	/* trap to a system call	*/

/* Traps used to modify the PSR */
#define U_TRAP_DISABLE		0x10	/* to disable interrupts	*/
#define U_TRAP_RESTORE		0x11	/* to restore interrupts	*/
#define U_TRAP_TO_USER_MODE	0x12	/* to set user/supervisor mode	*/
#define U_TRAP_FP_OnOff		0x13	/* to turn floating pt unit on/off */
#define U_TRAP_CP_OnOff		0x14	/* to turn coprocessor on/off	 */


