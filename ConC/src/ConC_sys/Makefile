#	{ConC-dir}/src/ConC_sys/Makefile
#

.SUFFIXES: .c .o .s

INCLUDE= ../../include

CFLAGS=	-c -O -I${INCLUDE}

CPP_FLAGS= -E -I${INCLUDE}

OBJS=	chprio.o create.o \
	freebuf.o \
	getbuf.o getitem.o getprio.o getstk.o gettid.o \
	initialize.o insert.o insertd.o interrupt.o io_server.o \
	mkpool.o \
	newqueue.o \
	panic.o pcount.o pcreate.o pdelete.o pinit.o \
	poolinit.o preceive.o preset.o psend.o ptclear.o \
	qdump.o queue.o \
	ready.o resched.o resume.o \
	scount.o screate.o sdelete.o serv_create.o sleep.o sleep10.o \
	sreset.o ssignal.o ssignaln.o stdump.o suspend.o swait.o \
	tdump.o tkill.o treceive.o trecvclr.o tsend.o tsendf.o \
	userret.o \
	wakeup.o \
	xdone.o \
	ctxsw.o \
	stdump_getap.o stdump_getfp.o stdump_getsp.o

CFILES=	chprio.c create.c \
	freebuf.c \
	getbuf.c getitem.c getprio.c getstk.c gettid.c \
	initialize.c insert.c insertd.c interrupt.c io_server.c \
	mkpool.c \
	newqueue.c \
	panic.c pcount.c pcreate.c pdelete.c pinit.c \
	poolinit.c preceive.c preset.c psend.c ptclear.c \
	qdump.c queue.c \
	ready.c resched.c resume.c scount.c \
	screate.c sdelete.c serv_create.c sleep.c sleep10.c \
	sreset.c ssignal.c ssignaln.c stdump.c suspend.c swait.c \
	tdump.c tkill.c treceive.c trecvclr.c tsend.c tsendf.c \
	userret.c \
	wakeup.c \
	xdone.c

SFILES=	ctxsw.s \
	stdump_getap.s stdump_getfp.s stdump_getsp.s

.s.o:
	/lib/cpp ${CPP_FLAGS} $*.s | ${AS} -o $*.o

.c.o:
	cc ${CFLAGS} $*.c

lib_flag:	${OBJS}
	touch lib_flag

clean:
	@echo "***  Cleaning src/ConC_sys ..."
	@/bin/rm -f ${OBJS} lib_flag

depend:
		@echo "***  Making src/ConC_sys depend  ..."
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
chprio.o: ${INCLUDE}/kernel.h
chprio.o: ${INCLUDE}/proc.h
chprio.o: ${INCLUDE}/errno.h
create.o: ${INCLUDE}/kernel.h
create.o: ${INCLUDE}/proc.h
create.o: ${INCLUDE}/mem.h
create.o: ${INCLUDE}/errno.h
freebuf.o: ${INCLUDE}/kernel.h
freebuf.o: ${INCLUDE}/bufpool.h
freebuf.o: ${INCLUDE}/errno.h
getbuf.o: ${INCLUDE}/kernel.h
getbuf.o: ${INCLUDE}/bufpool.h
getbuf.o: ${INCLUDE}/errno.h
getitem.o: ${INCLUDE}/kernel.h
getitem.o: ${INCLUDE}/q.h
getprio.o: ${INCLUDE}/kernel.h
getprio.o: ${INCLUDE}/proc.h
getprio.o: ${INCLUDE}/errno.h
getstk.o: ${INCLUDE}/kernel.h
getstk.o: ${INCLUDE}/mem.h
gettid.o: ${INCLUDE}/kernel.h
gettid.o: ${INCLUDE}/proc.h
initialize.o: ${INCLUDE}/kernel.h
initialize.o: ${INCLUDE}/proc.h
initialize.o: ${INCLUDE}/sem.h
initialize.o: ${INCLUDE}/mem.h
initialize.o: ${INCLUDE}/q.h
initialize.o: ${INCLUDE}/errno.h
initialize.o: ${INCLUDE}/stdio.h
insert.o: ${INCLUDE}/kernel.h
insert.o: ${INCLUDE}/q.h
insertd.o: ${INCLUDE}/kernel.h
insertd.o: ${INCLUDE}/q.h
interrupt.o: ${INCLUDE}/kernel.h
interrupt.o: ${INCLUDE}/proc.h
interrupt.o: ${INCLUDE}/syscall.h
io_server.o: ${INCLUDE}/kernel.h
io_server.o: ${INCLUDE}/syscall.h
io_server.o: ${INCLUDE}/stdio.h
io_server.o: ${INCLUDE}/errno.h
mkpool.o: ${INCLUDE}/kernel.h
mkpool.o: ${INCLUDE}/bufpool.h
mkpool.o: ${INCLUDE}/mem.h
mkpool.o: ${INCLUDE}/errno.h
newqueue.o: ${INCLUDE}/kernel.h
newqueue.o: ${INCLUDE}/q.h
pcount.o: ${INCLUDE}/kernel.h
pcount.o: ${INCLUDE}/ports.h
pcount.o: ${INCLUDE}/errno.h
pcreate.o: ${INCLUDE}/kernel.h
pcreate.o: ${INCLUDE}/ports.h
pcreate.o: ${INCLUDE}/errno.h
pdelete.o: ${INCLUDE}/kernel.h
pdelete.o: ${INCLUDE}/ports.h
pdelete.o: ${INCLUDE}/errno.h
pinit.o: ${INCLUDE}/kernel.h
pinit.o: ${INCLUDE}/ports.h
pinit.o: ${INCLUDE}/mem.h
poolinit.o: ${INCLUDE}/kernel.h
poolinit.o: ${INCLUDE}/bufpool.h
preceive.o: ${INCLUDE}/kernel.h
preceive.o: ${INCLUDE}/ports.h
preceive.o: ${INCLUDE}/errno.h
preset.o: ${INCLUDE}/kernel.h
preset.o: ${INCLUDE}/ports.h
preset.o: ${INCLUDE}/errno.h
psend.o: ${INCLUDE}/kernel.h
psend.o: ${INCLUDE}/ports.h
psend.o: ${INCLUDE}/errno.h
ptclear.o: ${INCLUDE}/kernel.h
ptclear.o: ${INCLUDE}/ports.h
qdump.o: ${INCLUDE}/kernel.h
qdump.o: ${INCLUDE}/proc.h
qdump.o: ${INCLUDE}/q.h
queue.o: ${INCLUDE}/kernel.h
queue.o: ${INCLUDE}/q.h
ready.o: ${INCLUDE}/kernel.h
ready.o: ${INCLUDE}/proc.h
ready.o: ${INCLUDE}/q.h
ready.o: ${INCLUDE}/errno.h
resched.o: ${INCLUDE}/kernel.h
resched.o: ${INCLUDE}/proc.h
resched.o: ${INCLUDE}/q.h
resume.o: ${INCLUDE}/kernel.h
resume.o: ${INCLUDE}/proc.h
resume.o: ${INCLUDE}/errno.h
scount.o: ${INCLUDE}/kernel.h
scount.o: ${INCLUDE}/sem.h
scount.o: ${INCLUDE}/errno.h
screate.o: ${INCLUDE}/kernel.h
screate.o: ${INCLUDE}/proc.h
screate.o: ${INCLUDE}/q.h
screate.o: ${INCLUDE}/sem.h
screate.o: ${INCLUDE}/errno.h
sdelete.o: ${INCLUDE}/kernel.h
sdelete.o: ${INCLUDE}/proc.h
sdelete.o: ${INCLUDE}/q.h
sdelete.o: ${INCLUDE}/sem.h
sdelete.o: ${INCLUDE}/errno.h
serv_create.o: ${INCLUDE}/kernel.h
serv_create.o: ${INCLUDE}/syscall.h
serv_create.o: ${INCLUDE}/stdio.h
sleep.o: ${INCLUDE}/kernel.h
sleep.o: ${INCLUDE}/proc.h
sleep.o: ${INCLUDE}/q.h
sleep.o: ${INCLUDE}/errno.h
sleep10.o: ${INCLUDE}/kernel.h
sleep10.o: ${INCLUDE}/proc.h
sleep10.o: ${INCLUDE}/q.h
sleep10.o: ${INCLUDE}/errno.h
sreset.o: ${INCLUDE}/kernel.h
sreset.o: ${INCLUDE}/proc.h
sreset.o: ${INCLUDE}/q.h
sreset.o: ${INCLUDE}/sem.h
sreset.o: ${INCLUDE}/errno.h
ssignal.o: ${INCLUDE}/kernel.h
ssignal.o: ${INCLUDE}/proc.h
ssignal.o: ${INCLUDE}/q.h
ssignal.o: ${INCLUDE}/sem.h
ssignal.o: ${INCLUDE}/errno.h
ssignaln.o: ${INCLUDE}/kernel.h
ssignaln.o: ${INCLUDE}/proc.h
ssignaln.o: ${INCLUDE}/q.h
ssignaln.o: ${INCLUDE}/sem.h
ssignaln.o: ${INCLUDE}/errno.h
stdump.o: ${INCLUDE}/kernel.h
stdump.o: ${INCLUDE}/proc.h
stdump.o: ${INCLUDE}/q.h
suspend.o: ${INCLUDE}/kernel.h
suspend.o: ${INCLUDE}/proc.h
suspend.o: ${INCLUDE}/errno.h
swait.o: ${INCLUDE}/kernel.h
swait.o: ${INCLUDE}/proc.h
swait.o: ${INCLUDE}/q.h
swait.o: ${INCLUDE}/sem.h
swait.o: ${INCLUDE}/errno.h
tdump.o: ${INCLUDE}/kernel.h
tdump.o: ${INCLUDE}/proc.h
tdump.o: ${INCLUDE}/q.h
tkill.o: ${INCLUDE}/kernel.h
tkill.o: ${INCLUDE}/proc.h
tkill.o: ${INCLUDE}/sem.h
tkill.o: ${INCLUDE}/mem.h
tkill.o: ${INCLUDE}/errno.h
treceive.o: ${INCLUDE}/kernel.h
treceive.o: ${INCLUDE}/proc.h
trecvclr.o: ${INCLUDE}/kernel.h
trecvclr.o: ${INCLUDE}/proc.h
tsend.o: ${INCLUDE}/kernel.h
tsend.o: ${INCLUDE}/proc.h
tsend.o: ${INCLUDE}/errno.h
tsendf.o: ${INCLUDE}/kernel.h
tsendf.o: ${INCLUDE}/proc.h
tsendf.o: ${INCLUDE}/errno.h
userret.o: ${INCLUDE}/kernel.h
userret.o: ${INCLUDE}/proc.h
wakeup.o: ${INCLUDE}/kernel.h
wakeup.o: ${INCLUDE}/proc.h
wakeup.o: ${INCLUDE}/q.h
xdone.o: ${INCLUDE}/kernel.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
