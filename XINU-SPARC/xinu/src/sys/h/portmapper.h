/* portmapper.h */

/* Portmapper specs */
#define PMAP_PORT		111
#define PMAP_PROG		100000
#define PMAP_VERS		2

/* Procedures that the portmapper runs */
#define PMAP_PROC_NAMES	  {"NULL","SET","UNSET","GETPORT","DUMP","CALLIT",0};
#define PMAP_PROC_NULL		0
#define PMAP_PROC_SET		1
#define PMAP_PROC_UNSET		2
#define PMAP_PROC_GETPORT 	3
#define PMAP_PROC_DUMP		4
#define PMAP_PROC_CALLIT	5

