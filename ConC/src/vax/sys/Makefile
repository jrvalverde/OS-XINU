#	{ConC-dir}/src/vax/sys/Makefile
#

.SUFFIXES: .s .c .o

DIR=src/vax/sys

INCLUDE= ../../../include

CFLAGS=	-c -O -I${INCLUDE}

CPP_FLAGS= -E -I${INCLUDE}

OBJS=	sys_Ovadvise.o sys_Ovfork.o \
	sys__exit.o \
	sys_accept.o sys_access.o sys_acct.o sys_adjtime.o \
	sys_bind.o sys_brk.o \
	sys_cerror.o sys_chdir.o sys_chmod.o sys_chown.o \
	sys_chroot.o sys_close.o sys_connect.o sys_creat.o \
	sys_dup.o sys_dup2.o \
	sys_execl.o sys_execle.o sys_exect.o sys_execv.o sys_execve.o \
	sys_fchmod.o sys_fchown.o sys_fcntl.o sys_flock.o \
	sys_fork.o sys_fstat.o sys_fsync.o sys_ftruncate.o \
	sys_getdtablesize.o sys_getegid.o sys_geteuid.o sys_getgid.o \
	sys_getgroups.o sys_gethostid.o sys_gethostname.o sys_getitimer.o \
	sys_getpagesize.o sys_getpeername.o sys_getpgrp.o sys_getpid.o \
	sys_getppid.o sys_getpriority.o sys_getrlimit.o sys_getrusage.o \
	sys_getsockname.o sys_getsockopt.o sys_gettimeofday.o sys_getuid.o \
	sys_ioctl.o \
	sys_kill.o sys_killpg.o \
	sys_link.o sys_listen.o sys_lseek.o sys_lstat.o \
	sys_mkdir.o sys_mknod.o sys_mount.o \
	sys_open.o \
	sys_pipe.o sys_profil.o sys_ptrace.o \
	sys_quota.o \
	sys_read.o sys_readlink.o sys_readv.o sys_reboot.o \
	sys_recv.o sys_recvfrom.o sys_recvmsg.o sys_rename.o sys_rmdir.o \
	sys_sbrk.o sys_select.o sys_send.o sys_sendmsg.o sys_sendto.o \
	sys_setgroups.o sys_sethostid.o sys_sethostname.o sys_setitimer.o \
	sys_setpgrp.o sys_setpriority.o sys_setquota.o sys_setregid.o \
	sys_setreuid.o sys_setrlimit.o sys_setsockopt.o sys_settimeofday.o \
	sys_shutdown.o sys_sigblock.o sys_sigpause.o sys_sigreturn.o \
	sys_sigsetmask.o sys_sigstack.o sys_sigvec.o sys_socket.o \
	sys_socketpair.o sys_stat.o sys_swapon.o sys_symlink.o \
	sys_sync.o sys_syscall.o \
	sys_truncate.o \
	sys_umask.o sys_umount.o sys_unlink.o sys_utimes.o \
	sys_vhangup.o \
	sys_wait.o sys_wait3.o sys_write.o sys_writev.o

CFILES=	sys_Ovadvise.c sys_Ovfork.c \
	sys__exit.c \
	sys_accept.c sys_access.c sys_acct.c sys_adjtime.c \
	sys_bind.c sys_brk.c \
	sys_cerror.c sys_chdir.c sys_chmod.c sys_chown.c sys_chroot.c \
	sys_close.c sys_connect.c sys_creat.c \
	sys_dup.c sys_dup2.c \
	sys_execl.c sys_execle.c sys_exect.c sys_execv.c sys_execve.c \
	sys_fchmod.c sys_fchown.c sys_fcntl.c sys_flock.c sys_fork.c \
	sys_fstat.c sys_fsync.c sys_ftruncate.c \
	sys_getdtablesize.c sys_getegid.c sys_geteuid.c sys_getgid.c \
	sys_getgroups.c sys_gethostid.c sys_gethostname.c sys_getitimer.c \
	sys_getpagesize.c sys_getpeername.c sys_getpgrp.c sys_getpid.c \
	sys_getppid.c sys_getpriority.c sys_getrlimit.c sys_getrusage.c \
	sys_getsockname.c sys_getsockopt.c sys_gettimeofday.c sys_getuid.c \
	sys_ioctl.c \
	sys_kill.c sys_killpg.c \
	sys_link.c sys_listen.c sys_lseek.c sys_lstat.c \
	sys_mkdir.c sys_mknod.c sys_mount.c \
	sys_open.c \
	sys_pipe.c sys_profil.c sys_ptrace.c \
	sys_quota.c \
	sys_read.c sys_readlink.c sys_readv.c sys_reboot.c sys_recv.c \
	sys_recvfrom.c sys_recvmsg.c sys_rename.c sys_rmdir.c \
	sys_sbrk.c sys_select.c sys_send.c sys_sendmsg.c sys_sendto.c \
	sys_setgroups.c sys_sethostid.c sys_sethostname.c sys_setitimer.c \
	sys_setpgrp.c sys_setpriority.c sys_setquota.c sys_setregid.c \
	sys_setreuid.c sys_setrlimit.c sys_setsockopt.c sys_settimeofday.c \
	sys_shutdown.c sys_sigblock.c sys_sigpause.c sys_sigreturn.c \
	sys_sigsetmask.c sys_sigstack.c sys_sigvec.c sys_socket.c \
	sys_socketpair.c sys_stat.c sys_swapon.c sys_symlink.c \
	sys_sync.c sys_syscall.c \
	sys_truncate.c \
	sys_umask.c sys_umount.c sys_unlink.c sys_utimes.c \
	sys_vhangup.c \
	sys_wait.c sys_wait3.c sys_write.c sys_writev.c

SFILES= 

.c.o:
	/lib/cpp ${CPP_FLAGS} $*.c | ${AS} -o $*.o

lib_flag:	${OBJS}
	touch lib_flag

clean:
	@echo "***  Cleaning ${DIR} ..."
	@/bin/rm -f ${OBJS} lib_flag

depend:
		@echo "***  Making ${DIR} depend  ..."
		@echo '^#include[ 	]*(<(DUMMY.x' > .ils
		@/bin/ls ${INCLUDE} >> .ils
		@echo 'DUMMY.x)>|")' >> .ils
		@(egrep -f .ils ${CFILES} ${SFILES} ${INCLUDE}/*.h	\
		| sed	-e 's/</"$${INCLUDE}\//;s/>/"/'			\
			-e 's/:[^"]*"\([^"]*\)".*/: \1/'		\
		| sed	-e 's/\.c/.o/'					\
			-e 's/\.s/.o/' >makedep)
		@echo '/^# DO NOT DELETE	THIS LINE/+2,$$d' >eddep
		@echo '$$r makedep' >>eddep
		@echo 'w' >>eddep
		@cp Makefile .,oldmake
		@cp Makefile ,Makefile
		@ed - ,Makefile < eddep
		@/bin/rm eddep makedep .ils
		@echo '#	DEPENDENCIES MUST END AT END OF	FILE' >>,Makefile
		@echo '#	IF YOU PUT STUFF HERE IT WILL GO AWAY'>>,Makefile
		@echo '#	see make depend	above'>>,Makefile
		@/bin/mv ,Makefile Makefile

# DO NOT DELETE	THIS LINE
# DEPENDENCIES MUST END	AT END OF FILE
sys_Ovadvise.o: SYS.h
sys__exit.o: SYS.h
sys_accept.o: SYS.h
sys_access.o: SYS.h
sys_acct.o: SYS.h
sys_adjtime.o: SYS.h
sys_bind.o: SYS.h
sys_brk.o: SYS.h
sys_cerror.o: SYS.h
sys_chdir.o: SYS.h
sys_chmod.o: SYS.h
sys_chown.o: SYS.h
sys_chroot.o: SYS.h
sys_close.o: SYS.h
sys_connect.o: SYS.h
sys_creat.o: SYS.h
sys_dup.o: SYS.h
sys_dup2.o: SYS.h
sys_execl.o: SYS.h
sys_execle.o: SYS.h
sys_exect.o: SYS.h
sys_execv.o: SYS.h
sys_execve.o: SYS.h
sys_fchmod.o: SYS.h
sys_fchown.o: SYS.h
sys_fcntl.o: SYS.h
sys_flock.o: SYS.h
sys_fork.o: SYS.h
sys_fstat.o: SYS.h
sys_fsync.o: SYS.h
sys_ftruncate.o: SYS.h
sys_getdtablesize.o: SYS.h
sys_getegid.o: SYS.h
sys_geteuid.o: SYS.h
sys_getgid.o: SYS.h
sys_getgroups.o: SYS.h
sys_gethostid.o: SYS.h
sys_gethostname.o: SYS.h
sys_getitimer.o: SYS.h
sys_getpagesize.o: SYS.h
sys_getpeername.o: SYS.h
sys_getpgrp.o: SYS.h
sys_getpid.o: SYS.h
sys_getppid.o: SYS.h
sys_getpriority.o: SYS.h
sys_getrlimit.o: SYS.h
sys_getrusage.o: SYS.h
sys_getsockname.o: SYS.h
sys_getsockopt.o: SYS.h
sys_gettimeofday.o: SYS.h
sys_getuid.o: SYS.h
sys_ioctl.o: SYS.h
sys_kill.o: SYS.h
sys_killpg.o: SYS.h
sys_link.o: SYS.h
sys_listen.o: SYS.h
sys_lseek.o: SYS.h
sys_lstat.o: SYS.h
sys_mkdir.o: SYS.h
sys_mknod.o: SYS.h
sys_mount.o: SYS.h
sys_open.o: SYS.h
sys_pipe.o: SYS.h
sys_profil.o: SYS.h
sys_ptrace.o: SYS.h
sys_quota.o: SYS.h
sys_read.o: SYS.h
sys_readlink.o: SYS.h
sys_readv.o: SYS.h
sys_reboot.o: SYS.h
sys_recv.o: SYS.h
sys_recvfrom.o: SYS.h
sys_recvmsg.o: SYS.h
sys_rename.o: SYS.h
sys_rmdir.o: SYS.h
sys_sbrk.o: SYS.h
sys_select.o: SYS.h
sys_send.o: SYS.h
sys_sendmsg.o: SYS.h
sys_sendto.o: SYS.h
sys_setgroups.o: SYS.h
sys_sethostid.o: SYS.h
sys_sethostname.o: SYS.h
sys_setitimer.o: SYS.h
sys_setpgrp.o: SYS.h
sys_setpriority.o: SYS.h
sys_setquota.o: SYS.h
sys_setregid.o: SYS.h
sys_setreuid.o: SYS.h
sys_setrlimit.o: SYS.h
sys_setsockopt.o: SYS.h
sys_settimeofday.o: SYS.h
sys_shutdown.o: SYS.h
sys_sigblock.o: SYS.h
sys_sigpause.o: SYS.h
sys_sigreturn.o: SYS.h
sys_sigsetmask.o: SYS.h
sys_sigstack.o: SYS.h
sys_sigvec.o: SYS.h
sys_socket.o: SYS.h
sys_socketpair.o: SYS.h
sys_stat.o: SYS.h
sys_swapon.o: SYS.h
sys_symlink.o: SYS.h
sys_sync.o: SYS.h
sys_syscall.o: SYS.h
sys_truncate.o: SYS.h
sys_umask.o: SYS.h
sys_umount.o: SYS.h
sys_unlink.o: SYS.h
sys_utimes.o: SYS.h
sys_vhangup.o: SYS.h
sys_wait.o: SYS.h
sys_wait3.o: SYS.h
sys_write.o: SYS.h
sys_writev.o: SYS.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
