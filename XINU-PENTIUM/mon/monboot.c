/* 
 * boot_xinu.c - Bootstrap Xinu from network
 * 
 * Author:	John c. Lin
 * 		Dept. of Computer Science
 * 		Purdue University
 * Date:	Fri Jun 16 15:42:26 1995
 */
#include <./mon/monnetwork.h>
#include <./mon/monitor.h>

int mon_boot_state;
struct netif *mon_eth_pni = &mon_nif[0];	/* network interface */
int mon_boot_try = 0;
int mon_clktime = 0;		/* tick once per sec. for retx purpose */
int mon_timeout = 0;		/* when to send a retx; 0: stop	retx */

/*#define VERBOSE*/
#define PRINTERR

/*-------------------------------------------------------------------------
 * boot_xinu() - 
 *-------------------------------------------------------------------------
 */
int boot_xinu()
{
    STATWORD ps;

    mon_boot_try = mon_timeout = 0;
    mon_boot_state = BEGIN_BOOTP;

    while (1) {
	switch (mon_boot_state) {
	case BOOTP_RETX:
	    if (++mon_boot_try > MON_MAX_TRY) {
#ifdef PRINTERR
		kprintf("Eorror: No reply to BOOTP request.\n");
#endif
		return (SYSERR);
	    }
	    /* fall through */
	    
	case BEGIN_BOOTP:
	      mon_boot_state = BOOTP_REQ_SENT;
#ifdef VERBOSE
	      kprintf("Send BOOTP request....\n");
#endif
	      mon_bootp_request();
	      mon_timeout = mon_clktime + 2;    /* start retx timer */
	      break;

	case BOOTP_REQ_SENT:
	    /*
	     * wait for reply
	     */
	    break;

	case TFTP_RETX:
	    if (++mon_boot_try > MON_MAX_TRY) {
#ifdef PRINTERR
		kprintf("Eorror: No reply to TFTP request.\n");
#endif
		return(SYSERR);
	    }
	    /* fall through */
	    
	case BEGIN_TFTP:
#ifdef VERBOSE
	    kprintf("Send TFTP request....\n");
#endif
	    mon_tftp_req();
	    mon_boot_state = TFTP_REQ_SENT;
	    mon_timeout = mon_clktime + 2;    /* start retx timer */
	    mon_tftp_block = 1;
	    mon_tftp_bytes = 0;
	    mon_tftp_retx = 0;
	    break;

	case TFTP_REQ_SENT:
	    /*
	     * wait for transfer to complete
	     */
	    break;

	case BOOT_DONE:
	    /*
	     * Jump to loaded image
	     */
	    disable(ps);
            jumptobootcode();
	    break;

	case BOOT_ERROR:
	    return(SYSERR);
	    
	default:
#ifdef PRINTERR	    
	    kprintf("xinuboot: illegal state = %d\n", mon_boot_state);
#endif
	    break;	    
	}
    }
}
