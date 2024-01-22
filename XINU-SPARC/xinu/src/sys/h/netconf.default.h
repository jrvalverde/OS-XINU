/* netconf.default.h */
#include <conf.h>

/* Default values for network configuration */
#define GATEWAY_DEFAULT     "128.211.1.254"	/* Gateway address	*/
#define TSERVER_DEFAULT     "128.211.1.3:37"	/* Time server address	*/
#define NSERVER_DEFAULT     "128.10.2.5:53"	/* Domain Name server address*/
#define MYIPADDR_DEFAULT    "128.211.1.11"	/* my default IP address */

/* we recommend you use Sun NFS for paging and file storage in this release */

#ifdef NFS_PAGING
#define RSERVER_DEFAULT     "128.211.1.8" 	/* Remote file server address*/
#define PGSERVER_DEFAULT    RSERVER_DEFAULT	/* address of page server */
#else
#define RSERVER_DEFAULT     "128.211.1.54:2001" /* Remote file server address*/
#define PGSERVER_DEFAULT    "128.10.3.106:2020"	/* address of page server */
#endif

/* Default values for network ports */
#define TIME_PORT	37
#define NAME_PORT	53
#define RFS_PORT	2001
#define PAGE_PORT	2020
