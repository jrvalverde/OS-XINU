#
#  Makefile for the book version of Xinu
#

.SUFFIXES:.o .c .s
DIR	=	/usr/Xinu
BINDIR	=	$(DIR)/bin
INCDIR	=	$(DIR)/include
CONFIG	=	./config
CC	=	$(BINDIR)/cc11
CFLAGS	=	-O -c -I../h
AFLAGS	=	-u $(INCDIR)/sys.s
AS	=	$(BINDIR)/as11
LD	=	$(BINDIR)/ld11
AR	=	/bin/ar
RANLIB	=	$(BINDIR)/ranlib11
LORDER	=	$(BINDIR)/lorder11
LIB	=	$(DIR)/lib
DOWNLOAD=	$(BINDIR)/download
SIZE11	=	$(BINDIR)/size11
CPRINT	=	$(BINDIR)/cprint
ODT	=	$(BINDIR)/odt
.c.o:
		$(CC) $(CFLAGS)	$<
.s.o:
		$(CC) $(CFLAGS)	$<

CFILES=		access.c adump.c arp_in.c arpfind.c	\
		arpinit.c				\
		blkcopy.c blkequ.c bpdump.c		\
		chprio.c ckmode.c clkinit.c close.c	\
		conf.c control.c create.c		\
		devdump.c dfalloc.c dfdsrch.c dgalloc.c	\
		dgclose.c dgcntl.c dgdump.c dginit.c	\
		dgmcntl.c dgmopen.c dgparse.c dgread.c	\
		dgwrite.c dot2ip.c dscntl.c dsinit.c	\
		dsinter.c dskbcpy.c dskenq.c dskqopt.c	\
		dskstrt.c dsksync.c dsopen.c dsread.c	\
		dsseek.c dswrite.c dudir.c dumkfs.c	\
		ethinit.c ethinter.c ethread.c ethstrt.c\
		ethrstrt.c ethwrite.c ethwstrt.c	\
		freebuf.c freemem.c			\
		getaddr.c getbuf.c getc.c getitem.c	\
		getmem.c getname.c getnet.c getpath.c	\
		getpid.c getprio.c getstk.c gettime.c	\
		getutim.c				\
		ibclear.c ibfree.c ibget.c iblfree.c	\
		ibnew.c ibput.c icmp_in.c init.c	\
		initialize.c insert.c insertd.c ioerr.c	\
		ioinit.c ionull.c ip2name.c ip_in.c	\
		ipsend.c				\
		kill.c kprintf.c			\
		lfclose.c lfgetc.c lfinit.c lfputc.c	\
		lfread.c lfsdfree.c lfseek.c lfsetup.c	\
		lfsflush.c lfsnewd.c lfwrite.c		\
		mark.c mdump.c mkarp.c mkpool.c mount.c	\
		naminit.c nammap.c namopen.c namrepl.c	\
		ndump.c netdump.c netin.c netinit.c	\
		netnum.c netout.c newqueue.c nqalloc.c	\
		open.c					\
		pcount.c pcreate.c pdelete.c pinit.c	\
		poolinit.c prdump.c preceive.c preset.c	\
		psend.c ptclear.c putc.c		\
		qdump.c queue.c				\
		rarp_in.c read.c ready.c receive.c	\
		recvclr.c recvtim.c remove.c rename.c	\
		resched.c resume.c rfalloc.c rfclose.c	\
		rfcntl.c rfdump.c rfgetc.c rfinit.c	\
		rfio.c rfmkpac.c rfopen.c rfputc.c	\
		rfread.c rfseek.c rfsend.c rfwrite.c	\
		route.c rwho.c rwhod.c rwhoind.c	\
		scount.c screate.c sdelete.c seek.c	\
		send.c sendf.c setdev.c setnok.c	\
		signal.c signaln.c sleep.c sleep10.c	\
		sndrarp.c sreset.c ssclock.c suspend.c	\
		tdump.c test.c ttycntl.c ttygetc.c	\
		ttyiin.c ttyinit.c ttyoin.c ttyopen.c	\
		ttyputc.c ttyread.c ttywrite.c		\
		udpecho.c udpnxtp.c udpsend.c unmount.c	\
		unsleep.c userret.c			\
		wait.c wakeup.c write.c			\
		xdone.c

SFILES=		cksum.s clkint.s csv.s ctxsw.s		\
		ioint.s					\
		lowcore.s				\
		netutil.s				\
		panic.s					\
		setclkr.s sizmem.s startup.s

OFILES=		access.o adump.o arp_in.o arpfind.o	\
		arpinit.o				\
		blkcopy.o blkequ.o bpdump.o		\
		chprio.o ckmode.o cksum.o clkinit.o	\
		clkint.o close.o control.o create.o	\
		csv.o ctxsw.o				\
		devdump.o dfalloc.o dfdsrch.o dgalloc.o	\
		dgclose.o dgcntl.o dgdump.o dginit.o	\
		dgmcntl.o dgmopen.o dgparse.o dgread.o	\
		dgwrite.o dot2ip.o dscntl.o dsinit.o	\
		dsinter.o dskbcpy.o dskenq.o dskqopt.o	\
		dskstrt.o dsksync.o dsopen.o dsread.o	\
		dsseek.o dswrite.o dudir.o dumkfs.o	\
		ethinit.o ethinter.o ethread.o ethstrt.o\
		ethrstrt.o ethwrite.o ethwstrt.o	\
		freebuf.o freemem.o			\
		getaddr.o getbuf.o getc.o getitem.o	\
		getmem.o getname.o getnet.o getpath.o	\
		getpid.o getprio.o getstk.o gettime.o	\
		getutim.o				\
		ibclear.o ibfree.o ibget.o iblfree.o	\
		ibnew.o ibput.o icmp_in.o init.o	\
		insert.o insertd.o ioerr.o ioinit.o	\
		ioint.o ionull.o ip2name.o ip_in.o	\
		ipsend.o				\
		kill.o kprintf.o			\
		lfclose.o lfgetc.o lfinit.o lfputc.o	\
		lfread.o lfsdfree.o lfseek.o lfsetup.o	\
		lfsflush.o lfsnewd.o lfwrite.o		\
		mark.o mdump.o mkarp.o mkpool.o mount.o	\
		naminit.o nammap.o namopen.o namrepl.o	\
		ndump.o netdump.o netin.o netinit.o	\
		netnum.o netout.o netutil.o newqueue.o	\
		nqalloc.o				\
		open.o					\
		panic.o pcount.o pcreate.o pdelete.o	\
		pinit.o poolinit.o prdump.o preceive.o	\
		preset.o psend.o ptclear.o putc.o	\
		qdump.o queue.o				\
		rarp_in.o read.o ready.o receive.o	\
		recvclr.o recvtim.o remove.o rename.o	\
		resched.o resume.o rfalloc.o rfclose.o	\
		rfcntl.o rfdump.o rfgetc.o rfinit.o	\
		rfio.o rfmkpac.o rfopen.o rfputc.o	\
		rfread.o rfseek.o rfsend.o rfwrite.o	\
		route.o rwho.o rwhod.o rwhoind.o	\
		scount.o screate.o sdelete.o seek.o	\
		send.o sendf.o setclkr.o setdev.o	\
		setnok.o sh.o signal.o signaln.o	\
		sizmem.o sleep.o sleep10.o sndrarp.o	\
		sreset.o ssclock.o suspend.o		\
		tdump.o test.o ttycntl.o ttygetc.o	\
		ttyiin.o ttyinit.o ttyoin.o ttyopen.o	\
		ttyputc.o ttyread.o ttywrite.o		\
		udpecho.o udpnxtp.o udpsend.o unmount.o	\
		unsleep.o userret.o			\
		wait.o wakeup.o write.o			\
		xdone.o

XOFILES=	lowcore.o startup.o initialize.o conf.o

HFILES=		../h/a.out.h ../h/arp.h ../h/bufpool.h	\
		../h/cmd.h ../h/conf.h ../h/core11.h	\
		../h/ctype.h ../h/date.h ../h/deqna.h	\
		../h/dgram.h ../h/dir.h ../h/disk.h	\
		../h/domain.h ../h/dtc.h ../h/ether.h	\
		../h/file.h ../h/fserver.h ../h/hosts.h	\
		../h/iblock.h ../h/icmp.h ../h/io.h	\
		../h/ip.h ../h/kernel.h ../h/lfile.h	\
		../h/mark.h ../h/mem.h ../h/name.h	\
		../h/net.h ../h/network.h ../h/ports.h	\
		../h/proc.h ../h/q.h ../h/rfile.h	\
		../h/rwho.h ../h/sem.h ../h/shell.h	\
		../h/sleep.h ../h/slu.h ../h/tty.h	\
		../h/udp.h ../h/unix-rwho.h ../h/xebec.h

all:		libx.a xinu.o

install:	libx.a xinu.o
		cp libx.a $(LIB)/libx.a
		$(RANLIB) $(LIB)/libx.a
		cp xinu.o $(LIB)

clean:
		-/bin/rm -rf *.o ,* .,* ../h/,* ../h/.,*
		-/bin/rm -rf libx.a a.out conf.c ../h/conf.h core make.out

everything:	a.out xref checkup sizes lint

down:		a.out
		$(DOWNLOAD) -s

d:		a.out
		$(DOWNLOAD) -s
		$(ODT)

lint:		,lint

,lint:		$(CFILES)
		lint -n -I../h $(CFILES) | ./Flint > ,lint

print:
		$(CPRINT) -c Cover ../h/*.h *.c *.s

listings:	xref sizes
		$(CPRINT) -c Cover Configuration ../h/*.h *.c *.s \
		    ,xref ,sizes ,ssizes Makefile

checkup:
		./Checkup

sizes:		,sizes ,ssizes

,sizes:		$(OFILES) $(XOFILES) a.out
		$(SIZE11) *.o a.out > ,sizes

,ssizes:	$(CFILES) $(SFILES) $(HFILES)
		./Dosizes

sh.o:
		(cd shell; make install)

libx.a:		$(OFILES)
		/bin/rm -f  libx.a
		$(AR) cr libx.a	$(OFILES)
		$(RANLIB) libx.a

xinu.o:		$(XOFILES)
		$(LD) -r $(XOFILES) -o xinu.o

a.out:		libx.a xinu.o test.o
		$(LD) -S -e start xinu.o test.o libx.a $(LIB)/libxc.a libx.a
		./Mknlist ,nm

nm:		,nm

,nm:		a.out
		./Mknlist ,nm

tags:		,tags

,tags:		$(CFILES) $(HFILES)
		ctags $(CFILES)	$(HFILES)
		/bin/mv tags ,tags

xref:		,xref

,xref:		,tags
		./Doxref

../h/conf.h:	Configuration
		$(CONFIG)
		/bin/rm $(INCDIR)/conf.h
		/bin/ln ../h/conf.h $(INCDIR)/conf.h

conf.c:		Configuration
		$(CONFIG)

book:		.formatted
		./Doform l

.formatted:	.hupdate .cupdate .supdate .shupdate .Cupdate
		touch .formatted

.supdate:	$(SFILES)
		./Doform s
		touch .supdate

.shupdate:	sh.o
		./Doform sh
		touch .shupdate

.cupdate:	$(CFILES)
		./Doform c
		touch .cupdate

.hupdate:	$(HFILES)
		./Doform h
		touch .hupdate

.Cupdate:	Configuration
		./Doform C
		touch .Cupdate

depend:
		grep '^#include' *.c ../h/*.h			\
		| sed	-e 's/</"..\/h\//;s/>/"/'		\
			-e 's/:[^"]*"\([^"]*\)".*/: \1/'	\
		| sed	's/\.c/.o/' >makedep
		echo '/^# DO NOT DELETE	THIS LINE/+2,$$d' >eddep
		echo '$$r makedep' >>eddep
		echo 'w' >>eddep
		cp Makefile .,oldmake
		cp Makefile ,Makefile
		ed - ,Makefile < eddep
		/bin/rm eddep makedep
		echo '#	DEPENDENCIES MUST END AT END OF	FILE' >>,Makefile
		echo '#	IF YOU PUT STUFF HERE IT WILL GO AWAY'>>,Makefile
		echo '#	see make depend	above'>>,Makefile
		/bin/mv ,Makefile Makefile

# DO NOT DELETE	THIS LINE
# DEPENDENCIES MUST END	AT END OF FILE
access.o: ../h/conf.h
access.o: ../h/kernel.h
access.o: ../h/file.h
access.o: ../h/name.h
adump.o: ../h/conf.h
adump.o: ../h/kernel.h
adump.o: ../h/proc.h
adump.o: ../h/network.h
arp_in.o: ../h/conf.h
arp_in.o: ../h/kernel.h
arp_in.o: ../h/proc.h
arp_in.o: ../h/network.h
arpfind.o: ../h/conf.h
arpfind.o: ../h/kernel.h
arpfind.o: ../h/proc.h
arpfind.o: ../h/network.h
arpinit.o: ../h/conf.h
arpinit.o: ../h/kernel.h
arpinit.o: ../h/proc.h
arpinit.o: ../h/network.h
blkcopy.o: ../h/kernel.h
blkequ.o: ../h/kernel.h
bpdump.o: ../h/conf.h
bpdump.o: ../h/kernel.h
bpdump.o: ../h/mark.h
bpdump.o: ../h/bufpool.h
chprio.o: ../h/conf.h
chprio.o: ../h/kernel.h
chprio.o: ../h/proc.h
ckmode.o: ../h/conf.h
ckmode.o: ../h/kernel.h
ckmode.o: ../h/file.h
clkinit.o: ../h/conf.h
clkinit.o: ../h/kernel.h
clkinit.o: ../h/sleep.h
close.o: ../h/conf.h
close.o: ../h/kernel.h
close.o: ../h/io.h
conf.o: ../h/conf.h
control.o: ../h/conf.h
control.o: ../h/kernel.h
control.o: ../h/io.h
create.o: ../h/conf.h
create.o: ../h/kernel.h
create.o: ../h/proc.h
create.o: ../h/mem.h
create.o: ../h/io.h
devdump.o: ../h/conf.h
devdump.o: ../h/kernel.h
dfalloc.o: ../h/conf.h
dfalloc.o: ../h/kernel.h
dfalloc.o: ../h/disk.h
dfalloc.o: ../h/lfile.h
dfdsrch.o: ../h/conf.h
dfdsrch.o: ../h/kernel.h
dfdsrch.o: ../h/disk.h
dfdsrch.o: ../h/lfile.h
dfdsrch.o: ../h/dir.h
dgalloc.o: ../h/conf.h
dgalloc.o: ../h/kernel.h
dgalloc.o: ../h/network.h
dgclose.o: ../h/conf.h
dgclose.o: ../h/kernel.h
dgclose.o: ../h/proc.h
dgclose.o: ../h/network.h
dgcntl.o: ../h/conf.h
dgcntl.o: ../h/kernel.h
dgcntl.o: ../h/network.h
dgdump.o: ../h/conf.h
dgdump.o: ../h/kernel.h
dgdump.o: ../h/network.h
dginit.o: ../h/conf.h
dginit.o: ../h/kernel.h
dginit.o: ../h/network.h
dgmcntl.o: ../h/conf.h
dgmcntl.o: ../h/kernel.h
dgmcntl.o: ../h/network.h
dgmopen.o: ../h/conf.h
dgmopen.o: ../h/kernel.h
dgmopen.o: ../h/network.h
dgparse.o: ../h/conf.h
dgparse.o: ../h/kernel.h
dgparse.o: ../h/network.h
dgparse.o: ../h/ctype.h
dgread.o: ../h/conf.h
dgread.o: ../h/kernel.h
dgread.o: ../h/proc.h
dgread.o: ../h/network.h
dgwrite.o: ../h/conf.h
dgwrite.o: ../h/kernel.h
dgwrite.o: ../h/network.h
dot2ip.o: ../h/conf.h
dot2ip.o: ../h/kernel.h
dot2ip.o: ../h/network.h
dscntl.o: ../h/conf.h
dscntl.o: ../h/kernel.h
dscntl.o: ../h/proc.h
dscntl.o: ../h/disk.h
dsinit.o: ../h/conf.h
dsinit.o: ../h/kernel.h
dsinit.o: ../h/disk.h
dsinter.o: ../h/conf.h
dsinter.o: ../h/kernel.h
dsinter.o: ../h/disk.h
dskbcpy.o: ../h/conf.h
dskbcpy.o: ../h/kernel.h
dskbcpy.o: ../h/disk.h
dskenq.o: ../h/conf.h
dskenq.o: ../h/kernel.h
dskenq.o: ../h/disk.h
dskqopt.o: ../h/conf.h
dskqopt.o: ../h/kernel.h
dskqopt.o: ../h/disk.h
dskstrt.o: ../h/conf.h
dskstrt.o: ../h/kernel.h
dskstrt.o: ../h/disk.h
dsksync.o: ../h/conf.h
dsksync.o: ../h/kernel.h
dsksync.o: ../h/proc.h
dsksync.o: ../h/disk.h
dsopen.o: ../h/conf.h
dsopen.o: ../h/kernel.h
dsopen.o: ../h/disk.h
dsopen.o: ../h/lfile.h
dsopen.o: ../h/dir.h
dsread.o: ../h/conf.h
dsread.o: ../h/kernel.h
dsread.o: ../h/proc.h
dsread.o: ../h/disk.h
dsseek.o: ../h/conf.h
dsseek.o: ../h/kernel.h
dsseek.o: ../h/proc.h
dsseek.o: ../h/disk.h
dswrite.o: ../h/conf.h
dswrite.o: ../h/kernel.h
dswrite.o: ../h/proc.h
dswrite.o: ../h/disk.h
dudir.o: ../h/conf.h
dudir.o: ../h/kernel.h
dudir.o: ../h/io.h
dudir.o: ../h/disk.h
dudir.o: ../h/lfile.h
dudir.o: ../h/dir.h
dumkfs.o: ../h/conf.h
dumkfs.o: ../h/kernel.h
dumkfs.o: ../h/disk.h
dumkfs.o: ../h/lfile.h
dumkfs.o: ../h/dir.h
ethinit.o: ../h/conf.h
ethinit.o: ../h/kernel.h
ethinit.o: ../h/network.h
ethinter.o: ../h/conf.h
ethinter.o: ../h/kernel.h
ethinter.o: ../h/proc.h
ethinter.o: ../h/network.h
ethread.o: ../h/conf.h
ethread.o: ../h/kernel.h
ethread.o: ../h/proc.h
ethread.o: ../h/network.h
ethrstrt.o: ../h/conf.h
ethrstrt.o: ../h/kernel.h
ethrstrt.o: ../h/network.h
ethstrt.o: ../h/conf.h
ethstrt.o: ../h/kernel.h
ethstrt.o: ../h/network.h
ethwrite.o: ../h/conf.h
ethwrite.o: ../h/kernel.h
ethwrite.o: ../h/proc.h
ethwrite.o: ../h/network.h
ethwstrt.o: ../h/conf.h
ethwstrt.o: ../h/kernel.h
ethwstrt.o: ../h/proc.h
ethwstrt.o: ../h/network.h
freebuf.o: ../h/conf.h
freebuf.o: ../h/kernel.h
freebuf.o: ../h/mark.h
freebuf.o: ../h/bufpool.h
freemem.o: ../h/conf.h
freemem.o: ../h/kernel.h
freemem.o: ../h/mem.h
getaddr.o: ../h/conf.h
getaddr.o: ../h/kernel.h
getaddr.o: ../h/network.h
getbuf.o: ../h/conf.h
getbuf.o: ../h/kernel.h
getbuf.o: ../h/mark.h
getbuf.o: ../h/bufpool.h
getc.o: ../h/conf.h
getc.o: ../h/kernel.h
getc.o: ../h/io.h
getitem.o: ../h/conf.h
getitem.o: ../h/kernel.h
getitem.o: ../h/q.h
getmem.o: ../h/conf.h
getmem.o: ../h/kernel.h
getmem.o: ../h/mem.h
getname.o: ../h/conf.h
getname.o: ../h/kernel.h
getname.o: ../h/network.h
getnet.o: ../h/conf.h
getnet.o: ../h/kernel.h
getnet.o: ../h/network.h
getpath.o: ../h/conf.h
getpath.o: ../h/kernel.h
getpath.o: ../h/proc.h
getpath.o: ../h/network.h
getpid.o: ../h/conf.h
getpid.o: ../h/kernel.h
getpid.o: ../h/proc.h
getprio.o: ../h/conf.h
getprio.o: ../h/kernel.h
getprio.o: ../h/proc.h
getstk.o: ../h/conf.h
getstk.o: ../h/kernel.h
getstk.o: ../h/mem.h
gettime.o: ../h/conf.h
gettime.o: ../h/kernel.h
gettime.o: ../h/date.h
getutim.o: ../h/conf.h
getutim.o: ../h/kernel.h
getutim.o: ../h/sleep.h
getutim.o: ../h/date.h
getutim.o: ../h/network.h
ibclear.o: ../h/conf.h
ibclear.o: ../h/kernel.h
ibclear.o: ../h/disk.h
ibclear.o: ../h/lfile.h
ibclear.o: ../h/dir.h
ibfree.o: ../h/conf.h
ibfree.o: ../h/kernel.h
ibfree.o: ../h/io.h
ibfree.o: ../h/disk.h
ibfree.o: ../h/lfile.h
ibfree.o: ../h/dir.h
ibget.o: ../h/conf.h
ibget.o: ../h/kernel.h
ibget.o: ../h/disk.h
ibget.o: ../h/lfile.h
ibget.o: ../h/dir.h
iblfree.o: ../h/conf.h
iblfree.o: ../h/kernel.h
iblfree.o: ../h/io.h
iblfree.o: ../h/disk.h
iblfree.o: ../h/lfile.h
iblfree.o: ../h/dir.h
ibnew.o: ../h/conf.h
ibnew.o: ../h/kernel.h
ibnew.o: ../h/io.h
ibnew.o: ../h/disk.h
ibnew.o: ../h/lfile.h
ibnew.o: ../h/dir.h
ibput.o: ../h/conf.h
ibput.o: ../h/kernel.h
ibput.o: ../h/io.h
ibput.o: ../h/disk.h
ibput.o: ../h/lfile.h
ibput.o: ../h/dir.h
icmp_in.o: ../h/conf.h
icmp_in.o: ../h/kernel.h
icmp_in.o: ../h/network.h
init.o: ../h/conf.h
init.o: ../h/kernel.h
init.o: ../h/io.h
initialize.o: ../h/conf.h
initialize.o: ../h/kernel.h
initialize.o: ../h/proc.h
initialize.o: ../h/sem.h
initialize.o: ../h/sleep.h
initialize.o: ../h/mem.h
initialize.o: ../h/tty.h
initialize.o: ../h/q.h
initialize.o: ../h/io.h
initialize.o: ../h/disk.h
initialize.o: ../h/network.h
insert.o: ../h/conf.h
insert.o: ../h/kernel.h
insert.o: ../h/q.h
insertd.o: ../h/conf.h
insertd.o: ../h/kernel.h
insertd.o: ../h/q.h
ioerr.o: ../h/conf.h
ioerr.o: ../h/kernel.h
ioinit.o: ../h/conf.h
ioinit.o: ../h/kernel.h
ioinit.o: ../h/io.h
ionull.o: ../h/conf.h
ionull.o: ../h/kernel.h
ip2name.o: ../h/conf.h
ip2name.o: ../h/kernel.h
ip2name.o: ../h/network.h
ip_in.o: ../h/conf.h
ip_in.o: ../h/kernel.h
ip_in.o: ../h/proc.h
ip_in.o: ../h/network.h
ipsend.o: ../h/conf.h
ipsend.o: ../h/kernel.h
ipsend.o: ../h/network.h
kill.o: ../h/conf.h
kill.o: ../h/kernel.h
kill.o: ../h/proc.h
kill.o: ../h/sem.h
kill.o: ../h/mem.h
kill.o: ../h/io.h
kprintf.o: ../h/conf.h
kprintf.o: ../h/kernel.h
kprintf.o: ../h/io.h
kprintf.o: ../h/slu.h
kprintf.o: ../h/tty.h
lfclose.o: ../h/conf.h
lfclose.o: ../h/kernel.h
lfclose.o: ../h/proc.h
lfclose.o: ../h/disk.h
lfclose.o: ../h/lfile.h
lfclose.o: ../h/dir.h
lfgetc.o: ../h/conf.h
lfgetc.o: ../h/kernel.h
lfgetc.o: ../h/proc.h
lfgetc.o: ../h/disk.h
lfgetc.o: ../h/lfile.h
lfgetc.o: ../h/dir.h
lfinit.o: ../h/conf.h
lfinit.o: ../h/kernel.h
lfinit.o: ../h/disk.h
lfinit.o: ../h/lfile.h
lfputc.o: ../h/conf.h
lfputc.o: ../h/kernel.h
lfputc.o: ../h/proc.h
lfputc.o: ../h/disk.h
lfputc.o: ../h/lfile.h
lfputc.o: ../h/dir.h
lfread.o: ../h/conf.h
lfread.o: ../h/kernel.h
lfread.o: ../h/disk.h
lfread.o: ../h/lfile.h
lfread.o: ../h/dir.h
lfsdfree.o: ../h/conf.h
lfsdfree.o: ../h/kernel.h
lfsdfree.o: ../h/disk.h
lfsdfree.o: ../h/lfile.h
lfsdfree.o: ../h/dir.h
lfseek.o: ../h/conf.h
lfseek.o: ../h/kernel.h
lfseek.o: ../h/disk.h
lfseek.o: ../h/lfile.h
lfseek.o: ../h/dir.h
lfsetup.o: ../h/conf.h
lfsetup.o: ../h/kernel.h
lfsetup.o: ../h/disk.h
lfsetup.o: ../h/lfile.h
lfsetup.o: ../h/dir.h
lfsflush.o: ../h/conf.h
lfsflush.o: ../h/kernel.h
lfsflush.o: ../h/disk.h
lfsflush.o: ../h/lfile.h
lfsflush.o: ../h/dir.h
lfsnewd.o: ../h/conf.h
lfsnewd.o: ../h/kernel.h
lfsnewd.o: ../h/disk.h
lfsnewd.o: ../h/lfile.h
lfsnewd.o: ../h/dir.h
lfwrite.o: ../h/conf.h
lfwrite.o: ../h/kernel.h
mark.o: ../h/conf.h
mark.o: ../h/kernel.h
mark.o: ../h/mark.h
mdump.o: ../h/conf.h
mdump.o: ../h/kernel.h
mdump.o: ../h/proc.h
mdump.o: ../h/mem.h
mkarp.o: ../h/conf.h
mkarp.o: ../h/kernel.h
mkarp.o: ../h/network.h
mkpool.o: ../h/conf.h
mkpool.o: ../h/kernel.h
mkpool.o: ../h/mark.h
mkpool.o: ../h/bufpool.h
mount.o: ../h/conf.h
mount.o: ../h/kernel.h
mount.o: ../h/name.h
naminit.o: ../h/conf.h
naminit.o: ../h/kernel.h
naminit.o: ../h/name.h
nammap.o: ../h/conf.h
nammap.o: ../h/kernel.h
nammap.o: ../h/name.h
namopen.o: ../h/conf.h
namopen.o: ../h/kernel.h
namopen.o: ../h/name.h
namrepl.o: ../h/conf.h
namrepl.o: ../h/kernel.h
namrepl.o: ../h/name.h
ndump.o: ../h/conf.h
ndump.o: ../h/kernel.h
ndump.o: ../h/io.h
ndump.o: ../h/name.h
netdump.o: ../h/conf.h
netdump.o: ../h/kernel.h
netdump.o: ../h/network.h
netin.o: ../h/conf.h
netin.o: ../h/kernel.h
netin.o: ../h/proc.h
netin.o: ../h/network.h
netinit.o: ../h/conf.h
netinit.o: ../h/kernel.h
netinit.o: ../h/sleep.h
netinit.o: ../h/network.h
netnum.o: ../h/conf.h
netnum.o: ../h/kernel.h
netnum.o: ../h/network.h
netout.o: ../h/conf.h
netout.o: ../h/kernel.h
netout.o: ../h/network.h
newqueue.o: ../h/conf.h
newqueue.o: ../h/kernel.h
newqueue.o: ../h/q.h
nqalloc.o: ../h/conf.h
nqalloc.o: ../h/kernel.h
nqalloc.o: ../h/proc.h
nqalloc.o: ../h/network.h
open.o: ../h/conf.h
open.o: ../h/kernel.h
open.o: ../h/io.h
pcount.o: ../h/conf.h
pcount.o: ../h/kernel.h
pcount.o: ../h/mark.h
pcount.o: ../h/ports.h
pcreate.o: ../h/conf.h
pcreate.o: ../h/kernel.h
pcreate.o: ../h/mark.h
pcreate.o: ../h/ports.h
pdelete.o: ../h/conf.h
pdelete.o: ../h/kernel.h
pdelete.o: ../h/mark.h
pdelete.o: ../h/ports.h
pinit.o: ../h/conf.h
pinit.o: ../h/kernel.h
pinit.o: ../h/mark.h
pinit.o: ../h/ports.h
poolinit.o: ../h/conf.h
poolinit.o: ../h/kernel.h
poolinit.o: ../h/mark.h
poolinit.o: ../h/bufpool.h
prdump.o: ../h/conf.h
prdump.o: ../h/kernel.h
prdump.o: ../h/proc.h
prdump.o: ../h/q.h
preceive.o: ../h/conf.h
preceive.o: ../h/kernel.h
preceive.o: ../h/mark.h
preceive.o: ../h/ports.h
preset.o: ../h/conf.h
preset.o: ../h/kernel.h
preset.o: ../h/mark.h
preset.o: ../h/ports.h
psend.o: ../h/conf.h
psend.o: ../h/kernel.h
psend.o: ../h/mark.h
psend.o: ../h/ports.h
ptclear.o: ../h/conf.h
ptclear.o: ../h/kernel.h
ptclear.o: ../h/mark.h
ptclear.o: ../h/ports.h
putc.o: ../h/conf.h
putc.o: ../h/kernel.h
putc.o: ../h/io.h
qdump.o: ../h/conf.h
qdump.o: ../h/kernel.h
qdump.o: ../h/proc.h
qdump.o: ../h/q.h
queue.o: ../h/conf.h
queue.o: ../h/kernel.h
queue.o: ../h/q.h
rarp_in.o: ../h/conf.h
rarp_in.o: ../h/kernel.h
rarp_in.o: ../h/proc.h
rarp_in.o: ../h/network.h
read.o: ../h/conf.h
read.o: ../h/kernel.h
read.o: ../h/io.h
ready.o: ../h/conf.h
ready.o: ../h/kernel.h
ready.o: ../h/proc.h
ready.o: ../h/q.h
receive.o: ../h/conf.h
receive.o: ../h/kernel.h
receive.o: ../h/proc.h
recvclr.o: ../h/conf.h
recvclr.o: ../h/kernel.h
recvclr.o: ../h/proc.h
recvtim.o: ../h/conf.h
recvtim.o: ../h/kernel.h
recvtim.o: ../h/proc.h
recvtim.o: ../h/q.h
recvtim.o: ../h/sleep.h
remove.o: ../h/conf.h
remove.o: ../h/kernel.h
remove.o: ../h/file.h
remove.o: ../h/name.h
rename.o: ../h/conf.h
rename.o: ../h/kernel.h
rename.o: ../h/file.h
rename.o: ../h/name.h
resched.o: ../h/conf.h
resched.o: ../h/kernel.h
resched.o: ../h/proc.h
resched.o: ../h/q.h
resume.o: ../h/conf.h
resume.o: ../h/kernel.h
resume.o: ../h/proc.h
rfalloc.o: ../h/conf.h
rfalloc.o: ../h/kernel.h
rfalloc.o: ../h/fserver.h
rfalloc.o: ../h/rfile.h
rfclose.o: ../h/conf.h
rfclose.o: ../h/kernel.h
rfclose.o: ../h/network.h
rfcntl.o: ../h/conf.h
rfcntl.o: ../h/kernel.h
rfcntl.o: ../h/network.h
rfdump.o: ../h/conf.h
rfdump.o: ../h/kernel.h
rfdump.o: ../h/fserver.h
rfdump.o: ../h/rfile.h
rfgetc.o: ../h/conf.h
rfgetc.o: ../h/kernel.h
rfgetc.o: ../h/network.h
rfinit.o: ../h/conf.h
rfinit.o: ../h/kernel.h
rfinit.o: ../h/fserver.h
rfinit.o: ../h/rfile.h
rfio.o: ../h/conf.h
rfio.o: ../h/kernel.h
rfio.o: ../h/network.h
rfmkpac.o: ../h/conf.h
rfmkpac.o: ../h/kernel.h
rfmkpac.o: ../h/network.h
rfopen.o: ../h/conf.h
rfopen.o: ../h/kernel.h
rfopen.o: ../h/network.h
rfputc.o: ../h/conf.h
rfputc.o: ../h/kernel.h
rfputc.o: ../h/network.h
rfread.o: ../h/conf.h
rfread.o: ../h/kernel.h
rfread.o: ../h/network.h
rfseek.o: ../h/conf.h
rfseek.o: ../h/kernel.h
rfseek.o: ../h/fserver.h
rfseek.o: ../h/rfile.h
rfsend.o: ../h/conf.h
rfsend.o: ../h/kernel.h
rfsend.o: ../h/network.h
rfwrite.o: ../h/conf.h
rfwrite.o: ../h/kernel.h
rfwrite.o: ../h/network.h
route.o: ../h/conf.h
route.o: ../h/kernel.h
route.o: ../h/network.h
rwho.o: ../h/conf.h
rwho.o: ../h/kernel.h
rwho.o: ../h/network.h
rwho.o: ../h/rwho.h
rwhod.o: ../h/conf.h
rwhod.o: ../h/kernel.h
rwhod.o: ../h/network.h
rwhod.o: ../h/rwho.h
rwhod.o: ../h/shell.h
rwhoind.o: ../h/conf.h
rwhoind.o: ../h/kernel.h
rwhoind.o: ../h/network.h
rwhoind.o: ../h/rwho.h
scount.o: ../h/conf.h
scount.o: ../h/kernel.h
scount.o: ../h/sem.h
screate.o: ../h/conf.h
screate.o: ../h/kernel.h
screate.o: ../h/proc.h
screate.o: ../h/q.h
screate.o: ../h/sem.h
sdelete.o: ../h/conf.h
sdelete.o: ../h/kernel.h
sdelete.o: ../h/proc.h
sdelete.o: ../h/q.h
sdelete.o: ../h/sem.h
seek.o: ../h/conf.h
seek.o: ../h/kernel.h
seek.o: ../h/io.h
send.o: ../h/conf.h
send.o: ../h/kernel.h
send.o: ../h/proc.h
sendf.o: ../h/conf.h
sendf.o: ../h/kernel.h
sendf.o: ../h/proc.h
setdev.o: ../h/conf.h
setdev.o: ../h/kernel.h
setdev.o: ../h/proc.h
setnok.o: ../h/conf.h
setnok.o: ../h/kernel.h
setnok.o: ../h/proc.h
signal.o: ../h/conf.h
signal.o: ../h/kernel.h
signal.o: ../h/proc.h
signal.o: ../h/q.h
signal.o: ../h/sem.h
signaln.o: ../h/conf.h
signaln.o: ../h/kernel.h
signaln.o: ../h/proc.h
signaln.o: ../h/q.h
signaln.o: ../h/sem.h
sleep.o: ../h/conf.h
sleep.o: ../h/kernel.h
sleep.o: ../h/proc.h
sleep.o: ../h/q.h
sleep.o: ../h/sleep.h
sleep10.o: ../h/conf.h
sleep10.o: ../h/kernel.h
sleep10.o: ../h/proc.h
sleep10.o: ../h/q.h
sleep10.o: ../h/sleep.h
sndrarp.o: ../h/conf.h
sndrarp.o: ../h/kernel.h
sndrarp.o: ../h/network.h
sreset.o: ../h/conf.h
sreset.o: ../h/kernel.h
sreset.o: ../h/proc.h
sreset.o: ../h/q.h
sreset.o: ../h/sem.h
ssclock.o: ../h/conf.h
ssclock.o: ../h/kernel.h
ssclock.o: ../h/proc.h
ssclock.o: ../h/q.h
ssclock.o: ../h/sleep.h
suspend.o: ../h/conf.h
suspend.o: ../h/kernel.h
suspend.o: ../h/proc.h
tdump.o: ../h/conf.h
tdump.o: ../h/kernel.h
tdump.o: ../h/tty.h
test.o: ../h/conf.h
test.o: ../h/kernel.h
test.o: ../h/network.h
ttycntl.o: ../h/conf.h
ttycntl.o: ../h/kernel.h
ttycntl.o: ../h/tty.h
ttycntl.o: ../h/io.h
ttycntl.o: ../h/slu.h
ttygetc.o: ../h/conf.h
ttygetc.o: ../h/kernel.h
ttygetc.o: ../h/tty.h
ttygetc.o: ../h/io.h
ttyiin.o: ../h/conf.h
ttyiin.o: ../h/kernel.h
ttyiin.o: ../h/tty.h
ttyiin.o: ../h/io.h
ttyiin.o: ../h/slu.h
ttyinit.o: ../h/conf.h
ttyinit.o: ../h/kernel.h
ttyinit.o: ../h/proc.h
ttyinit.o: ../h/tty.h
ttyinit.o: ../h/io.h
ttyinit.o: ../h/slu.h
ttyoin.o: ../h/conf.h
ttyoin.o: ../h/kernel.h
ttyoin.o: ../h/tty.h
ttyoin.o: ../h/io.h
ttyoin.o: ../h/slu.h
ttyopen.o: ../h/conf.h
ttyopen.o: ../h/kernel.h
ttyputc.o: ../h/conf.h
ttyputc.o: ../h/kernel.h
ttyputc.o: ../h/tty.h
ttyputc.o: ../h/io.h
ttyputc.o: ../h/slu.h
ttyread.o: ../h/conf.h
ttyread.o: ../h/kernel.h
ttyread.o: ../h/tty.h
ttyread.o: ../h/io.h
ttywrite.o: ../h/conf.h
ttywrite.o: ../h/kernel.h
ttywrite.o: ../h/tty.h
ttywrite.o: ../h/io.h
ttywrite.o: ../h/slu.h
udpecho.o: ../h/conf.h
udpecho.o: ../h/kernel.h
udpecho.o: ../h/network.h
udpnxtp.o: ../h/conf.h
udpnxtp.o: ../h/kernel.h
udpnxtp.o: ../h/network.h
udpsend.o: ../h/conf.h
udpsend.o: ../h/kernel.h
udpsend.o: ../h/network.h
unmount.o: ../h/conf.h
unmount.o: ../h/kernel.h
unmount.o: ../h/name.h
unsleep.o: ../h/conf.h
unsleep.o: ../h/kernel.h
unsleep.o: ../h/proc.h
unsleep.o: ../h/q.h
unsleep.o: ../h/sleep.h
userret.o: ../h/conf.h
userret.o: ../h/kernel.h
wait.o: ../h/conf.h
wait.o: ../h/kernel.h
wait.o: ../h/proc.h
wait.o: ../h/q.h
wait.o: ../h/sem.h
wakeup.o: ../h/conf.h
wakeup.o: ../h/kernel.h
wakeup.o: ../h/proc.h
wakeup.o: ../h/q.h
wakeup.o: ../h/sleep.h
write.o: ../h/conf.h
write.o: ../h/kernel.h
write.o: ../h/io.h
../h/disk.h: ../h/xebec.h
../h/disk.h: ../h/dtc.h
../h/lfile.h: ../h/iblock.h
../h/lfile.h: ../h/file.h
../h/network.h: ../h/deqna.h
../h/network.h: ../h/ether.h
../h/network.h: ../h/ip.h
../h/network.h: ../h/icmp.h
../h/network.h: ../h/udp.h
../h/network.h: ../h/net.h
../h/network.h: ../h/dgram.h
../h/network.h: ../h/arp.h
../h/network.h: ../h/fserver.h
../h/network.h: ../h/rfile.h
../h/network.h: ../h/domain.h
../h/rfile.h: ../h/file.h
../h/shell.h: ../h/mark.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
