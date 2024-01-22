/* pmapd.h */

/* PMAPD process info: for daemon.h */
#define PMAPD           pmapd           /* pmapd daemon                 */
#define PMAPP           pmapp           /* process for handling calls   */
extern  int             PMAPD();	/* pmapd routine		*/
extern  int             PMAPP();	/* pmapp routine		*/
#define PMAPSTK         4096		/* stack size for PMAPD         */
#define PMAPPRI         20              /* PMAPD priority               */
#define PMAPDNAM        "pmapd"         /* name of PMAPD process        */
#define PMAPPNAM        "pmapp"         /* name of PMAPP process        */

