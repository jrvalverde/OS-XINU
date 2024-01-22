/* syscall.h */

/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)syscall.h	5.4 (Berkeley) 4/3/86
 */

#define	SYS_sys_exit		  1
#define	SYS_sys_fork		  2
#define	SYS_sys_read		  3
#define	SYS_sys_write		  4
#define	SYS_sys_open		  5
#define	SYS_sys_close		  6
				/*  7 is old: wait */
#define	SYS_sys_creat		  8
#define	SYS_sys_link		  9
#define	SYS_sys_unlink		 10
#define	SYS_sys_execv		 11
#define	SYS_sys_chdir		 12
				/* 13 is old: time */
#define	SYS_sys_mknod		 14
#define	SYS_sys_chmod		 15
#define	SYS_sys_chown		 16
				/* 17 is old: sbreak */
				/* 18 is old: stat */
#define	SYS_sys_lseek		 19
#define	SYS_sys_getpid		 20
#define	SYS_sys_mount		 21
#define	SYS_sys_umount		 22
				/* 23 is old: setuid */
#define	SYS_sys_getuid		 24
				/* 25 is old: stime */
#define	SYS_sys_ptrace		 26
				/* 27 is old: alarm */
				/* 28 is old: fstat */
				/* 29 is old: pause */
				/* 30 is old: utime */
				/* 31 is old: stty */
				/* 32 is old: gtty */
#define	SYS_sys_access		 33
				/* 34 is old: nice */
				/* 35 is old: ftime */
#define	SYS_sys_sync		 36
#define	SYS_sys_kill		 37
#define	SYS_sys_stat		 38
				/* 39 is old: setpgrp */
#define	SYS_sys_lstat		 40
#define	SYS_sys_dup		 41
#define	SYS_sys_pipe		 42
				/* 43 is old: times */
#define	SYS_sys_profil		 44
				/* 45 is unused */
				/* 46 is old: setgid */
#define	SYS_sys_getgid		 47
				/* 48 is old: sigsys */
				/* 49 is unused */
				/* 50 is unused */
#define	SYS_sys_acct		 51
				/* 52 is old: phys */
				/* 53 is old: syslock */
#define	SYS_sys_ioctl		 54
#define	SYS_sys_reboot		 55
				/* 56 is old: mpxchan */
#define	SYS_sys_symlink		 57
#define	SYS_sys_readlink	 58
#define	SYS_sys_execve		 59
#define	SYS_sys_umask		 60
#define	SYS_sys_chroot		 61
#define	SYS_sys_fstat		 62
				/* 63 is unused */
#define	SYS_sys_getpagesize 64
#define	SYS_sys_mremap	65
				/* 66 is old: vfork */
				/* 67 is old: vread */
				/* 68 is old: vwrite */
#define	SYS_sys_sbrk	69
#define	SYS_sys_sstk	70
#define	SYS_sys_mmap	71
				/* 72 is old: vadvise */
#define	SYS_sys_munmap		 73
#define	SYS_sys_mprotect	 74
#define	SYS_sys_madvise		 75
#define	SYS_sys_vhangup		 76
				/* 77 is old: vlimit */
#define	SYS_sys_mincore	78
#define	SYS_sys_getgroups	79
#define	SYS_sys_setgroups	80
#define	SYS_sys_getpgrp	81
#define	SYS_sys_setpgrp	82
#define	SYS_sys_setitimer	83
#define	SYS_sys_wait	84
#define	SYS_sys_swapon	85
#define	SYS_sys_getitimer	86
#define	SYS_sys_gethostname	87
#define	SYS_sys_sethostname	88
#define	SYS_sys_getdtablesize 89
#define	SYS_sys_dup2	90
#define	SYS_sys_getdopt	91
#define	SYS_sys_fcntl	92
#define	SYS_sys_select	93
#define	SYS_sys_setdopt	94
#define	SYS_sys_fsync	95
#define	SYS_sys_setpriority	96
#define	SYS_sys_socket	97
#define	SYS_sys_connect	98
#define	SYS_sys_accept	99
#define	SYS_sys_getpriority	100
#define	SYS_sys_send	101
#define	SYS_sys_recv	102
#define	SYS_sys_sigreturn	103
#define	SYS_sys_bind	104
#define	SYS_sys_setsockopt	105
#define	SYS_sys_listen	106
				/* 107 was vtimes */
#define	SYS_sys_sigvec	108
#define	SYS_sys_sigblock	109
#define	SYS_sys_sigsetmask	110
#define	SYS_sys_sigpause	111
#define	SYS_sys_sigstack	112
#define	SYS_sys_recvmsg	113
#define	SYS_sys_sendmsg	114
				/* 115 is old vtrace */
#define	SYS_sys_gettimeofday 	116
#define	SYS_sys_getrusage	117
#define	SYS_sys_getsockopt	118
				/* 119 is old resuba */
#define	SYS_sys_readv		120
#define	SYS_sys_writev		121
#define	SYS_sys_settimeofday 	122
#define	SYS_sys_fchown		123
#define	SYS_sys_fchmod		124
#define	SYS_sys_recvfrom	125
#define	SYS_sys_setreuid	126
#define	SYS_sys_setregid	127
#define	SYS_sys_rename		128
#define	SYS_sys_truncate	129
#define	SYS_sys_ftruncate	130
#define	SYS_sys_flock		131
				/* 132 is unused */
#define	SYS_sys_sendto		133
#define	SYS_sys_shutdown	134
#define	SYS_sys_socketpair	135
#define	SYS_sys_mkdir		136
#define	SYS_sys_rmdir		137
#define	SYS_sys_utimes		138
				/* 139 is unused */
#define	SYS_sys_adjtime	140
#define	SYS_sys_getpeername	141
#define	SYS_sys_gethostid	142
#define	SYS_sys_sethostid	143
#define	SYS_sys_getrlimit	144
#define	SYS_sys_setrlimit	145
#define	SYS_sys_killpg	146
				/* 147 is unused */
#define	SYS_sys_setquota	148
#define	SYS_sys_quota	149
#define	SYS_sys_getsockname	150
