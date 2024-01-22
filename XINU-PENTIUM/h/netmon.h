/* netmon.h */

#define	ST_BPSIZE	128		/* station buffer pool size	*/

struct station {
	int		st_ifnum;	/* interface number		*/
	Eaddr		st_ea;		/* Station's Ethernet Address	*/
	IPaddr		st_ipa;		/* Station's IP Address		*/
	unsigned long	st_scount;	/* source packet count		*/
	unsigned long	st_dcount;	/* destination packet count	*/
	struct station	*st_next;	/* next station			*/
};

struct netmon {
	int	nm_mutex;
	int	nm_pool;
};

extern struct netmon Netmon;
