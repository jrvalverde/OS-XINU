/* cmd.h */

/* Declarations for all commands known by the shell */

struct	cmdent	{			/* entry in command table	*/
	char	*cmdnam;		/* name of command		*/
	Bool	cbuiltin;		/* Is this a builtin command?	*/
	int	(*cproc)();		/* procedure that implements cmd*/
};

extern	int
	x_arp(),	x_bpool(),	x_cat(),	x_close(),
	x_conf(),	x_cp(),		x_creat(),	x_date(),
	x_devs(),	x_dg(),		x_dumper(),	x_echo(),
	x_exit(),	x_finger(),	x_fmdump(),
	x_help(),	x_ifstat(),	x_kill(),	x_exit(),
	x_mem(),	x_mount(),	x_mv(),		x_net(),
	x_net(),	x_ns(),		x_pgstats(),	x_printpgtbl(),
	x_ping(),	x_ps(),		x_reboot(),	x_rf(),
	x_rls(),	x_rm(),		x_route(),	x_routes(),
	x_run(),	x_uptime(),	x_sgmp(),	x_sleep(),
	x_date(),	x_timerq(),	x_unmou(),	x_uptime(),
	x_vmem(),	x_who(),	x_sem(),
    	x_pmem(),	x_exception(),	x_args(),
    	x_chprio(),	x_snmp(),	x_clock(),
    	x_X(),		x_rpcinfo(),	x_pmeg(),
    	x_rusers(),	x_nfs(),	x_rpc(),	x_df(),
    	x_nfsls(),	cadump(),	x_rtt();
	
	
/* Commands:	 name		Builtin?	procedure	*/

#define	CMDS 	"arp",		FALSE,		x_arp,		\
		"args",		FALSE,		x_args,		\
		"bpool",	FALSE,		x_bpool,	\
		"cache",	FALSE,		cadump,		\
		"cat",		FALSE,		x_cat,		\
		"chprio",	TRUE,		x_chprio,	\
		"clock",	FALSE,		x_clock,	\
		"close",	FALSE,		x_close,	\
		"conf",		FALSE,		x_conf,		\
		"cp",		FALSE,		x_cp,		\
		"date",		FALSE,		x_date,		\
		"devs",		FALSE,		x_devs,		\
		"df",		FALSE,		x_df,		\
		"dg",		FALSE,		x_dg,		\
/*		"dumper",	FALSE,		x_dumper,	\*/\
		"exc",		FALSE,		x_exception,	\
		"echo",		FALSE,		x_echo,		\
		"exit",		TRUE,		x_exit,		\
		"f",		FALSE,		x_finger,	\
		"finger",	FALSE,		x_finger,	\
		"fm",		FALSE,		x_fmdump,	\
		"fmdump",	FALSE,		x_fmdump,	\
		"help",		FALSE,		x_help,		\
		"host",		FALSE,		x_ns,		\
		"ifstat",	FALSE,		x_ifstat,	\
		"kill",		TRUE,		x_kill,		\
		"logout",	TRUE,		x_exit,		\
		"mem",		FALSE,		x_mem,		\
		"mount",	FALSE,		x_mount,	\
		"mv",		FALSE,		x_mv,		\
		"net",		FALSE,		x_net,		\
		"netstat",	FALSE,		x_net,		\
		"nfs",		FALSE,		x_nfs,		\
		"nfsls",	FALSE,		x_nfsls,	\
		"ns",		FALSE,		x_ns,		\
		"pgstats",	FALSE,		x_pgstats,	\
		"pgtbl",	FALSE,		x_printpgtbl,	\
		"ping",		FALSE,		x_ping,		\
		"pmem",		FALSE,		x_pmem,		\
		"pmeg",		FALSE,		x_pmeg,		\
		"ps",		FALSE,		x_ps,		\
		"reboot",	TRUE,		x_reboot,	\
		"rf",		FALSE,		x_rf,		\
		"rls",		FALSE,		x_rls,		\
		"rm",		FALSE,		x_rm,		\
		"route",	FALSE,		x_route,	\
		"routes",	FALSE,		x_routes,	\
		"rpc",		FALSE,		x_rpc,		\
		"rpcinfo",	FALSE,		x_rpcinfo,	\
		"run",		FALSE,		x_run,		\
		"ruptime",	FALSE,		x_uptime,	\
		"rusers",	FALSE,		x_rusers,	\
		"sem",		FALSE,		x_sem,		\
		"sgmp",		FALSE,		x_sgmp,		\
		"sleep",	FALSE,		x_sleep,	\
		"snmp",		FALSE,		x_snmp,		\
		"time",		FALSE,		x_date,		\
		"timerq",	FALSE,		x_timerq,	\
		"unmount",	FALSE,		x_unmou,	\
		"uptime",	FALSE,		x_uptime,	\
		"vmem",		FALSE,		x_vmem,		\
		"who",		FALSE,		x_who,		\
		"rtt",		FALSE,		x_rtt,		\
/*		"X",		FALSE,		x_X,		\*/\
/*		"X11",		FALSE,		x_X,		\*/\
		"?",		FALSE,		x_help


extern	struct	cmdent	cmds[];
