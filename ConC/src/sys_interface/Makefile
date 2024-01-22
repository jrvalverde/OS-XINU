#	{ConC-dir}/src/sys_interface/Makefile
#

.SUFFIXES: .c .o

INCLUDE= ../../include

CFLAGS=	-c -O -I${INCLUDE}

OBJS=	Flush_IO_Pending.o \
	Ovadvise.o Ovfork.o \
	_exit.o \
	accept.o access.o acct.o adjtime.o \
	bind.o brk.o \
	cerror.o chdir.o chmod.o chown.o chroot.o \
	close.o connect.o creat.o \
	dup.o dup2.o \
	execl.o execle.o exect.o execv.o execve.o \
	fchmod.o fchown.o fcntl.o flock.o fork.o \
	fstat.o fsync.o ftruncate.o \
	getdtablesize.o getegid.o geteuid.o getgid.o getgroups.o \
	gethostid.o gethostname.o getitimer.o getpagesize.o \
	getpeername.o getpgrp.o getpid.o getppid.o getpriority.o \
	getrlimit.o getrusage.o getsockname.o getsockopt.o \
	gettimeofday.o getuid.o \
	ioctl.o \
	kill.o killpg.o \
	link.o listen.o lseek.o lstat.o \
	mkdir.o mknod.o mount.o \
	open.o \
	pipe.o profil.o ptrace.o \
	quota.o \
	read.o readlink.o readv.o reboot.o recv.o \
	recvfrom.o recvmsg.o rename.o rmdir.o \
	sbrk.o select.o send.o sendmsg.o sendto.o setgroups.o \
	sethostid.o sethostname.o setitimer.o setpgrp.o setpriority.o \
	setquota.o setregid.o setreuid.o setrlimit.o setsockopt.o \
	settimeofday.o shutdown.o sigblock.o sigpause.o sigreturn.o sigsetmask.o \
	sigstack.o sigvec.o socket.o socketpair.o stat.o swapon.o \
	symlink.o sync.o syscall.o \
	truncate.o \
	umask.o umount.o unlink.o utimes.o \
	vhangup.o \
	wait.o wait3.o write.o writev.o

CFILES=	Flush_IO_Pending.c \
	Ovadvise.c Ovfork.c \
	_exit.c \
	accept.c access.c acct.c adjtime.c \
	bind.c brk.c \
	cerror.c chdir.c chmod.c chown.c chroot.c \
	close.c connect.c creat.c \
	dup.c dup2.c \
	execl.c execle.c exect.c execv.c execve.c \
	fchmod.c fchown.c fcntl.c flock.c fork.c \
	fstat.c fsync.c ftruncate.c \
	getdtablesize.c getegid.c geteuid.c getgid.c getgroups.c \
	gethostid.c gethostname.c getitimer.c getpagesize.c \
	getpeername.c getpgrp.c getpid.c getppid.c getpriority.c \
	getrlimit.c getrusage.c getsockname.c getsockopt.c \
	gettimeofday.c getuid.c \
	ioctl.c \
	kill.c killpg.c \
	link.c listen.c lseek.c lstat.c \
	mkdir.c mknod.c mount.c \
	open.c \
	pipe.c profil.c ptrace.c \
	quota.c \
	read.c readlink.c readv.c reboot.c recv.c \
	recvfrom.c recvmsg.c rename.c rmdir.c \
	sbrk.c select.c send.c sendmsg.c sendto.c setgroups.c \
	sethostid.c sethostname.c setitimer.c setpgrp.c setpriority.c \
	setquota.c setregid.c setreuid.c setrlimit.c setsockopt.c \
	settimeofday.c shutdown.c sigblock.c sigpause.c sigreturn.c sigsetmask.c \
	sigstack.c sigvec.c socket.c socketpair.c stat.c swapon.c \
	symlink.c sync.c syscall.c \
	truncate.c \
	umask.c umount.c unlink.c utimes.c \
	vhangup.c \
	wait.c wait3.c write.c writev.c

SFILES=

.c.o:
	cc ${CFLAGS} $*.c

lib_flag:	${OBJS}
	touch lib_flag

clean:
	@echo "***  Cleaning src/sys_interface ..."
	@/bin/rm -f ${OBJS} lib_flag

depend:
		@echo "***  Making src/sys_interface depend  ..."
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
Flush_IO_Pending.o: ${INCLUDE}/kernel.h
Flush_IO_Pending.o: ${INCLUDE}/syscall.h
Flush_IO_Pending.o: ${INCLUDE}/proc.h
Flush_IO_Pending.o: ${INCLUDE}/stdio.h
Flush_IO_Pending.o: ${INCLUDE}/errno.h
Ovadvise.o: ${INCLUDE}/kernel.h
Ovadvise.o: ${INCLUDE}/errno.h
Ovfork.o: ${INCLUDE}/kernel.h
Ovfork.o: ${INCLUDE}/errno.h
accept.o: ${INCLUDE}/kernel.h
accept.o: ${INCLUDE}/syscall.h
accept.o: ${INCLUDE}/proc.h
accept.o: ${INCLUDE}/stdio.h
accept.o: ${INCLUDE}/errno.h
bind.o: ${INCLUDE}/kernel.h
bind.o: ${INCLUDE}/syscall.h
bind.o: ${INCLUDE}/proc.h
bind.o: ${INCLUDE}/stdio.h
bind.o: ${INCLUDE}/errno.h
chdir.o: ${INCLUDE}/kernel.h
chdir.o: ${INCLUDE}/syscall.h
chdir.o: ${INCLUDE}/proc.h
chdir.o: ${INCLUDE}/stdio.h
close.o: ${INCLUDE}/kernel.h
close.o: ${INCLUDE}/syscall.h
close.o: ${INCLUDE}/proc.h
close.o: ${INCLUDE}/stdio.h
close.o: ${INCLUDE}/errno.h
connect.o: ${INCLUDE}/kernel.h
connect.o: ${INCLUDE}/syscall.h
connect.o: ${INCLUDE}/proc.h
connect.o: ${INCLUDE}/stdio.h
connect.o: ${INCLUDE}/errno.h
creat.o: ${INCLUDE}/kernel.h
creat.o: ${INCLUDE}/syscall.h
creat.o: ${INCLUDE}/proc.h
creat.o: ${INCLUDE}/stdio.h
dup.o: ${INCLUDE}/kernel.h
dup.o: ${INCLUDE}/syscall.h
dup.o: ${INCLUDE}/proc.h
dup.o: ${INCLUDE}/stdio.h
dup.o: ${INCLUDE}/errno.h
dup2.o: ${INCLUDE}/kernel.h
dup2.o: ${INCLUDE}/syscall.h
dup2.o: ${INCLUDE}/proc.h
dup2.o: ${INCLUDE}/stdio.h
dup2.o: ${INCLUDE}/errno.h
execl.o: ${INCLUDE}/kernel.h
execl.o: ${INCLUDE}/errno.h
execle.o: ${INCLUDE}/kernel.h
execle.o: ${INCLUDE}/errno.h
exect.o: ${INCLUDE}/kernel.h
exect.o: ${INCLUDE}/errno.h
execv.o: ${INCLUDE}/kernel.h
execv.o: ${INCLUDE}/errno.h
execve.o: ${INCLUDE}/kernel.h
execve.o: ${INCLUDE}/errno.h
fchmod.o: ${INCLUDE}/kernel.h
fchmod.o: ${INCLUDE}/syscall.h
fchmod.o: ${INCLUDE}/proc.h
fchmod.o: ${INCLUDE}/stdio.h
fchmod.o: ${INCLUDE}/errno.h
fchown.o: ${INCLUDE}/kernel.h
fchown.o: ${INCLUDE}/syscall.h
fchown.o: ${INCLUDE}/proc.h
fchown.o: ${INCLUDE}/stdio.h
fchown.o: ${INCLUDE}/errno.h
fcntl.o: ${INCLUDE}/kernel.h
fcntl.o: ${INCLUDE}/errno.h
flock.o: ${INCLUDE}/kernel.h
flock.o: ${INCLUDE}/errno.h
fork.o: ${INCLUDE}/kernel.h
fork.o: ${INCLUDE}/errno.h
fstat.o: ${INCLUDE}/kernel.h
fstat.o: ${INCLUDE}/syscall.h
fstat.o: ${INCLUDE}/proc.h
fstat.o: ${INCLUDE}/stdio.h
fstat.o: ${INCLUDE}/errno.h
fsync.o: ${INCLUDE}/kernel.h
fsync.o: ${INCLUDE}/syscall.h
fsync.o: ${INCLUDE}/proc.h
fsync.o: ${INCLUDE}/stdio.h
fsync.o: ${INCLUDE}/errno.h
ftruncate.o: ${INCLUDE}/kernel.h
ftruncate.o: ${INCLUDE}/syscall.h
ftruncate.o: ${INCLUDE}/proc.h
ftruncate.o: ${INCLUDE}/stdio.h
ftruncate.o: ${INCLUDE}/errno.h
getdtablesize.o: ${INCLUDE}/kernel.h
getitimer.o: ${INCLUDE}/kernel.h
getitimer.o: ${INCLUDE}/errno.h
getpeername.o: ${INCLUDE}/kernel.h
getpeername.o: ${INCLUDE}/syscall.h
getpeername.o: ${INCLUDE}/proc.h
getpeername.o: ${INCLUDE}/stdio.h
getpeername.o: ${INCLUDE}/errno.h
getsockname.o: ${INCLUDE}/kernel.h
getsockname.o: ${INCLUDE}/syscall.h
getsockname.o: ${INCLUDE}/proc.h
getsockname.o: ${INCLUDE}/stdio.h
getsockname.o: ${INCLUDE}/errno.h
getsockopt.o: ${INCLUDE}/kernel.h
getsockopt.o: ${INCLUDE}/syscall.h
getsockopt.o: ${INCLUDE}/proc.h
getsockopt.o: ${INCLUDE}/stdio.h
getsockopt.o: ${INCLUDE}/errno.h
ioctl.o: ${INCLUDE}/kernel.h
ioctl.o: ${INCLUDE}/syscall.h
ioctl.o: ${INCLUDE}/proc.h
ioctl.o: ${INCLUDE}/stdio.h
ioctl.o: ${INCLUDE}/errno.h
ioctl.o: ${INCLUDE}/C_ioctl.h
kill.o: ${INCLUDE}/kernel.h
kill.o: ${INCLUDE}/errno.h
killpg.o: ${INCLUDE}/kernel.h
killpg.o: ${INCLUDE}/errno.h
listen.o: ${INCLUDE}/kernel.h
listen.o: ${INCLUDE}/syscall.h
listen.o: ${INCLUDE}/proc.h
listen.o: ${INCLUDE}/stdio.h
listen.o: ${INCLUDE}/errno.h
lseek.o: ${INCLUDE}/kernel.h
lseek.o: ${INCLUDE}/syscall.h
lseek.o: ${INCLUDE}/proc.h
lseek.o: ${INCLUDE}/stdio.h
lseek.o: ${INCLUDE}/errno.h
open.o: ${INCLUDE}/kernel.h
open.o: ${INCLUDE}/syscall.h
open.o: ${INCLUDE}/proc.h
open.o: ${INCLUDE}/stdio.h
pipe.o: ${INCLUDE}/kernel.h
pipe.o: ${INCLUDE}/syscall.h
pipe.o: ${INCLUDE}/proc.h
pipe.o: ${INCLUDE}/stdio.h
profil.o: ${INCLUDE}/kernel.h
profil.o: ${INCLUDE}/errno.h
ptrace.o: ${INCLUDE}/kernel.h
ptrace.o: ${INCLUDE}/errno.h
read.o: ${INCLUDE}/kernel.h
read.o: ${INCLUDE}/syscall.h
read.o: ${INCLUDE}/proc.h
read.o: ${INCLUDE}/stdio.h
read.o: ${INCLUDE}/errno.h
readv.o: ${INCLUDE}/kernel.h
readv.o: ${INCLUDE}/syscall.h
readv.o: ${INCLUDE}/proc.h
readv.o: ${INCLUDE}/stdio.h
readv.o: ${INCLUDE}/errno.h
recv.o: ${INCLUDE}/kernel.h
recv.o: ${INCLUDE}/syscall.h
recv.o: ${INCLUDE}/proc.h
recv.o: ${INCLUDE}/stdio.h
recv.o: ${INCLUDE}/errno.h
recvfrom.o: ${INCLUDE}/kernel.h
recvfrom.o: ${INCLUDE}/syscall.h
recvfrom.o: ${INCLUDE}/proc.h
recvfrom.o: ${INCLUDE}/stdio.h
recvfrom.o: ${INCLUDE}/errno.h
recvmsg.o: ${INCLUDE}/kernel.h
recvmsg.o: ${INCLUDE}/syscall.h
recvmsg.o: ${INCLUDE}/proc.h
recvmsg.o: ${INCLUDE}/stdio.h
recvmsg.o: ${INCLUDE}/errno.h
select.o: ${INCLUDE}/kernel.h
select.o: ${INCLUDE}/errno.h
send.o: ${INCLUDE}/kernel.h
send.o: ${INCLUDE}/syscall.h
send.o: ${INCLUDE}/proc.h
send.o: ${INCLUDE}/stdio.h
send.o: ${INCLUDE}/errno.h
sendmsg.o: ${INCLUDE}/kernel.h
sendmsg.o: ${INCLUDE}/syscall.h
sendmsg.o: ${INCLUDE}/proc.h
sendmsg.o: ${INCLUDE}/stdio.h
sendmsg.o: ${INCLUDE}/errno.h
sendto.o: ${INCLUDE}/kernel.h
sendto.o: ${INCLUDE}/syscall.h
sendto.o: ${INCLUDE}/proc.h
sendto.o: ${INCLUDE}/stdio.h
sendto.o: ${INCLUDE}/errno.h
setitimer.o: ${INCLUDE}/kernel.h
setitimer.o: ${INCLUDE}/errno.h
setsockopt.o: ${INCLUDE}/kernel.h
setsockopt.o: ${INCLUDE}/syscall.h
setsockopt.o: ${INCLUDE}/proc.h
setsockopt.o: ${INCLUDE}/stdio.h
setsockopt.o: ${INCLUDE}/errno.h
shutdown.o: ${INCLUDE}/kernel.h
shutdown.o: ${INCLUDE}/syscall.h
shutdown.o: ${INCLUDE}/proc.h
shutdown.o: ${INCLUDE}/stdio.h
shutdown.o: ${INCLUDE}/errno.h
sigblock.o: ${INCLUDE}/kernel.h
sigblock.o: ${INCLUDE}/errno.h
sigpause.o: ${INCLUDE}/kernel.h
sigpause.o: ${INCLUDE}/errno.h
sigreturn.o: ${INCLUDE}/kernel.h
sigreturn.o: ${INCLUDE}/errno.h
sigsetmask.o: ${INCLUDE}/kernel.h
sigsetmask.o: ${INCLUDE}/errno.h
sigvec.o: ${INCLUDE}/kernel.h
sigvec.o: ${INCLUDE}/errno.h
socket.o: ${INCLUDE}/kernel.h
socket.o: ${INCLUDE}/syscall.h
socket.o: ${INCLUDE}/proc.h
socket.o: ${INCLUDE}/stdio.h
socketpair.o: ${INCLUDE}/kernel.h
socketpair.o: ${INCLUDE}/syscall.h
socketpair.o: ${INCLUDE}/proc.h
socketpair.o: ${INCLUDE}/stdio.h
sync.o: ${INCLUDE}/kernel.h
sync.o: ${INCLUDE}/syscall.h
sync.o: ${INCLUDE}/proc.h
sync.o: ${INCLUDE}/stdio.h
syscall.o: ${INCLUDE}/kernel.h
syscall.o: ${INCLUDE}/errno.h
umask.o: ${INCLUDE}/kernel.h
umask.o: ${INCLUDE}/syscall.h
umask.o: ${INCLUDE}/proc.h
umask.o: ${INCLUDE}/stdio.h
vhangup.o: ${INCLUDE}/kernel.h
vhangup.o: ${INCLUDE}/errno.h
wait.o: ${INCLUDE}/kernel.h
wait.o: ${INCLUDE}/errno.h
wait3.o: ${INCLUDE}/kernel.h
wait3.o: ${INCLUDE}/errno.h
write.o: ${INCLUDE}/kernel.h
write.o: ${INCLUDE}/syscall.h
write.o: ${INCLUDE}/proc.h
write.o: ${INCLUDE}/stdio.h
write.o: ${INCLUDE}/errno.h
writev.o: ${INCLUDE}/kernel.h
writev.o: ${INCLUDE}/syscall.h
writev.o: ${INCLUDE}/proc.h
writev.o: ${INCLUDE}/stdio.h
writev.o: ${INCLUDE}/errno.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
