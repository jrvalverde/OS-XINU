struct debug_type {
	char  *option;
	int   level;
	};

#define DEBUG_OPTIONS	"tftp",	0,	\
			"bootp", 0,	\
			"timeout", 1,	\
			"mcast", 0, \
			"netmon", 0	\

extern struct debug_type debug_opt[];
extern int debug_level(char *option);

