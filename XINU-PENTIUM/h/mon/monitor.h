/* for booting an image file from network */

/*
 * states
 */
#define BEGIN_BOOTP	10
#define BOOTP_REQ_SENT	11
#define BEGIN_TFTP	12
#define TFTP_REQ_SENT	13
#define BOOT_DONE	14
#define BOOT_ERROR	15
#define BOOTP_RETX      16
#define TFTP_RETX       17

#define MON_MAX_TRY	3

/* ARP related stuff */
#define MON_ARP_TSIZE	3 
extern struct   arpentry  mon_arptable[];

/* network related stuff */
extern  struct  netinfo mon_Net;

extern int mon_boot_state, mon_boot_try;
extern int mon_tftp_block, mon_tftp_bytes, mon_tftp_retx;
extern struct netif *mon_eth_pni; 	/* pointer to network interface */
extern IPaddr mon_tftp_server;
extern char mon_boot_fname[];
extern int mon_clktime, mon_timeout;	/* for retx purpose */
