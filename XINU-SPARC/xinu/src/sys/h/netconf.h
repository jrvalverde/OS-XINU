/* netconf.h */

#define NETCONF_DEFAULT         1
#define NETCONF_INTERACTIVE     2
#define NETCONF_BOOTP           3

#define DOT_ADDR_SIZE 32

struct netconf {
        Bool init;      /* has it been initialized */
        char gateway[DOT_ADDR_SIZE];
        char nserver[DOT_ADDR_SIZE];
        char pgserver[DOT_ADDR_SIZE];
        char rserver[DOT_ADDR_SIZE];
        char tserver[DOT_ADDR_SIZE];
        char myIPaddr[DOT_ADDR_SIZE];
};

extern int netconf_method;
extern struct netconf netconf;

/* constants for compatibility with the old software */
#define	TSERVER		netconf.tserver
#define	RSERVER		netconf.rserver
#define	NSERVER		netconf.nserver
#define PGSERVERADDR	netconf.pgserver

