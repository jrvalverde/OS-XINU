/*---------------------------------------------------------------------------
 * pgio.h - page in/out head file
 *---------------------------------------------------------------------------
 */

/*** constants and variable for paging stats ****/
#define	ITIMSIZE	500		/* size of intimes array	*/
#define OTIMSIZE	500		/* size of outtimes array	*/
extern	u_long	accintime;		/* accumulated page in time	*/
extern	u_long	accouttime;		/* accumulated page out time	*/
extern	u_long	intimes[];		/* array of in times		*/
extern	u_long	outtimes[];		/* array of out times		*/
extern	u_long	cintimes;		/* count of pgs read in		*/
extern	u_long	couttimes;		/* count of pgs sent out	*/

extern	u_long	avintime();		/* calc average page in time	*/
extern	u_long	avouttime();		/* calc average page out time	*/

extern int sendproc;			/* XINU paging sender thread	*/
extern int nfspgout_sem;		/* NFS page out semaphore 	*/
#define MAX_PAGEOUT     16

struct rqnode {                         /* request list node structure  */
        long    timestp;                /* time stamp                   */
        int     listlen;                /* for measurement              */
        int     sent;                   /* times the rqt has been sent  */
        struct  rqnode  *nextnd;        /* next request node ptr        */
        struct  rqnode  *prevnd;        /* previous request node ptr    */
        struct  message *msgptr;        /* point to associated message  */
        long    statvar;                /* var for statistical purposes */
};

extern struct rqnode rq[];              /* req list head/tail node array*/
