/* ktab.h */

/* Kernel table objects */
struct ktspec {
	char	objname[16];	/* object name visible to user		*/
	char	objdescr[64];	/* description of ktab object		*/
	int	u_entrysize;	/* user's arch indep entry size		*/
	int	k_entrysize;	/* kernel's arch dep entry size		*/
	int	maxrows;	/* # of rows in the object		*/
	int	maxcols;	/* # of columns in the object		*/
	char	*objbase;	/* object base address			*/
	int	(*wconvert)();	/* arch indep -> arc dep conversion	*/
	int	(*rconvert)();	/* arch dep -> arch indep conversion	*/
	int	opencount;	/* # of slaves using this object	*/
};

/* Kernel table pseudo-device control block */

struct ktblk {
	int 	kt_dnum;	/* device number for this device 	*/
	int 	kt_state;	/* whether this device is allocated 	*/
	struct 	ktspec *kt_spec;/* specification of ktab object		*/
	int	kt_currow;	/* current row				*/
	int	kt_curcol;	/* current column			*/
	int	kt_direct;	/* direction for next read/write	*/
	int	kt_mode;	/* mode : SINGLE or FILL		*/
	int	kt_prot;	/* read/write protection		*/
};

/* Constants for kt pseudo-device state. */
#define KT_FREE		0	/* this device is available		*/
#define KT_USED		1	/* this device is in use		*/

/* Constants for kt pseudo-device functions.  */
#define KT_SET		0	/* set a slave device characteristic	*/
#define KT_GET		1	/* get a slave device characteristic	*/
#define KT_ENTRY_SIZE	0	/* GET only: size of entry for object	*/
#define KT_MAX_ROWS	1	/* GET only: max rows for object	*/
#define KT_MAX_COLS	2	/* GET only: max columns for object	*/
#define KT_CUR_ROW	3	/* GET/SET:  current row		*/
#define KT_CUR_COL	4	/* GET/SET:  current column		*/
#define KT_MODE		5	/* GET/SET:  mode of oper.: see below	*/
#define KT_DIRECTION	6	/* GET/SET:  direction: see below	*/

/* Constants for kt pseudo-device mode */
#define KT_FILL_MODE	0	/* fill mode				*/
#define KT_SINGLE_MODE	1	/* single mode				*/

/* Constants for kt pseudo-device direction */
#define KT_ROW_INC	0	/* increment the curr. row after r/w	*/
#define KT_ROW_DEC	1	/* decrement 				*/
#define KT_COL_INC	2	/* increment the curr. col after r/w	*/
#define KT_COL_DEC	3	/* decrement 				*/

extern struct ktspec ktobjects[];
extern struct ktblk  kttab[];

