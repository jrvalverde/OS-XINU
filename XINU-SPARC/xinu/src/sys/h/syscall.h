/* syscall.h */

/* System call routine numbers */
#define		SYSC_ACCESS			0
#define		SYSC_ASCREATE			2
#define 	SYSC_CHPRIO			3
#define		SYSC_CLOSE			4
#define 	SYSC_CONTROL			6
#define		SYSC_CREATE			7
#define		SYSC_FREEHEAP			8
#define		SYSC_GETADDR			10
#define		SYSC_GETC			12
#define		SYSC_GETHEAP			14
#define		SYSC_GETNET			16
#define		SYSC_GETPID			18
#define		SYSC_GETPRIO			20
#define		SYSC_GETREALTIM			21
#define		SYSC_GETUTIM			22
#define		SYSC_GETASID			24
#define		SYSC_IP2NAME			26
#define		SYSC_KILL			27
#define		SYSC_LOGIN			28
#define 	SYSC_MOUNT			30
#define		SYSC_OPEN			32
#define		SYSC_PANIC			34
#define 	SYSC_PCOUNT			35
#define		SYSC_PCREATE			36
#define		SYSC_PDELETE			38
#define 	SYSC_PRECEIVE			40
#define		SYSC_PRESET			42
#define		SYSC_PROCREATE			43
#define		SYSC_PSEND			44
#define		SYSC_PUTC			46
#define		SYSC_READ			48	
#define		SYSC_RECEIVE			50
#define		SYSC_RECVCLR			52
#define		SYSC_RECVTIM			54
#define		SYSC_REMOVE			55
#define		SYSC_RENAME			56
#define		SYSC_RESUME			58
#define		SYSC_SCOUNT			60
#define		SYSC_SCREATE			62
#define		SYSC_SDELETE			64	
#define		SYSC_SEEK			65
#define		SYSC_SEND			66
#define		SYSC_SENDF			68
#define		SYSC_SETDEV			70
#define		SYSC_SETNOK			72
#define		SYSC_SHELL			74
#define		SYSC_SIGNAL			75
#define		SYSC_SIGNALN			76
#define		SYSC_SLEEP			78
#define		SYSC_SLEEP10			80
#define		SYSC_SPRINTF			82
#define		SYSC_SRESET			84
#define		SYSC_SUSPEND			85
#define		SYSC_TCREATE			86
#define		SYSC_UNMOUNT			88
#define		SYSC_UNSLEEP			90
#define		SYSC_USERRET			92 /* this doesn't work yet */
#define		SYSC_WAIT			94
#define		SYSC_WRITE			96
/* newly added - 6-6-90 */
#define		SYSC_NAME2IP			29
#define		SYSC_XINUEXECV			97
#define		SYSC_DVLOOKUP			9
/* newly added - 6/22/92 */
#define		SYSC_NOP			95
#define		SYSC_STRLEN			98
#define		SYSC_KPRINTF			99

#ifdef KERNEL
/* Size of the System Call Table */
#define		SYSCALNUM			100

/* System Call Trap Table */
extern		int		(*syscalf[])();
						/* syscalf is an array of
						   pointers to system
						   call functions */

/* Declarations of system call functions referenced */
extern		int		access();
extern		int		ascreate();
extern		int		chprio();
extern		int		close();
extern		int		control();
extern		int		create();
extern		int		freeheap();
extern		int		getaddr();
extern		int		getc();
extern		int		getheap();
extern		int		getnet();
extern		int		getpid();
extern		int		getasid();
extern		int		getprio();
extern		int		getrealtim();
extern		int		getutim();
extern		int		ip2name();
extern		int		kill();
extern		int		login();
extern		int		mark();
extern		int		mkpool();
extern		int		mount();
extern		int		open();
extern		int		panic();
extern		int		pcount();
extern		int		pcreate();
extern		int		pdelete();
extern		int		pinit();
extern		int		preceive();
extern		int		preset();
extern		int		procreate();
extern		int		psend();
extern		int		putc();
extern		int		read();
extern		int		receive();
extern		int		recvclr();
extern		int		recvtim();
extern		int		remove();
extern		int		rename();
extern		int		resume();
extern		int		scount();
extern		int		screate();
extern		int		sdelete();
extern		int		seek();
extern		int		send();
extern		int		sendf();
extern		int		setdev();
extern		int		setnok();
extern		int		shell();
extern		int		signal();
extern		int		signaln();
extern		int		sleep();
extern		int		sleep10();
extern		int		sreset();
extern		int		suspend();
extern		int		sysopen();
extern		int		tcreate();
extern		int		unmount();
extern		int		unsleep();
extern		int		userret(); /* this doesn't work yet */
extern		int		wait();
extern		int		write();

/* newly added - 6-6-90 */
extern		int		name2ip();
extern		int		xinu_execv();
extern		int		dvlookup();

/* newly added - 6/22/92 */
extern		int		null_system_call();
extern		int		strlen();
extern		int		kprintf();

#endif KERNEL
