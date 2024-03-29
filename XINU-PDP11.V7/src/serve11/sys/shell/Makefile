#
# Makefile for pseudo shell and associated commands
#

.SUFFIXES:.o .c .s
DIR	=	/usr/Xinu
BINDIR	=	$(DIR)/bin
CC	=	$(BINDIR)/cc11
CFLAGS	=	-O -c -I../../h
AFLAGS	=	-u $(DIR)/include/sys.s
AS	=	$(BINDIR)/as11
LD	=	$(BINDIR)/ld11
AR	=	/bin/ar
LORDER	=	$(BINDIR)/lorder11
RANLIB	=	$(BINDIR)/ranlib11
LIB	=	$(DIR)/lib
.c.o:
		$(CC) $(CFLAGS)	$<
.s.o:
		$(CC) $(CFLAGS)	$<

CFILES=		addarg.c ascdate.c login.c lexan.c	\
		shell.c					\
		x_bpool.c x_cat.c x_close.c x_cp.c	\
		x_create.c x_date.c x_devs.c x_dg.c	\
		x_echo.c x_exit.c x_help.c x_kill.c	\
		x_mem.c x_mount.c x_mv.c x_net.c	\
		x_ps.c x_reboot.c x_rf.c x_rm.c x_rls.c	\
		x_routes.c x_sleep.c x_snap.c		\
		x_unmou.c x_who.c

XOFILES=	x_bpool.o x_cat.o x_close.o x_cp.o	\
		x_create.o x_date.o x_devs.o x_dg.o	\
		x_echo.o x_exit.o x_help.o x_kill.o	\
		x_mem.o x_mount.o x_mv.o x_net.o	\
		x_ps.o x_reboot.o x_rf.o x_rm.o x_rls.o	\
		x_routes.o x_sleep.o x_snap.o		\
		x_unmou.o x_who.o

HFILES=		shell.h cmd.h

all:		sh.o

sh.o:		addarg.o ascdate.o login.o lexan.o	\
		shell.o libS.a
		$(LD) -r -o sh.o \
			addarg.o ascdate.o login.o	\
			lexan.o shell.o libS.a

libS.a:		$(XOFILES)
		rm -rf libS.a
		$(AR) cr libS.a $(XOFILES)
		$(RANLIB) libS.a

install:	sh.o
		cp sh.o ..

clean:
		-rm -rf *.o ,* .,* core make.out

everything:	all lint sizes xref

lint:		,lint

,lint:		$(CFILES)
		lint -n -I../../h $(CFILES) | Flint > ,lint

print:
		/usr/Xinu/cprint -c Cover *.h *.c

listings:	sizes xref
		/usr/Xinu/cprint -c Cover *.h *.c ,xref ,sizes ,ssizes Makefile

sizes:		,sizes ,ssizes

,sizes:		$(OFILES)
		/usr/Xinu/size11 *.o > ,sizes

,ssizes:	$(CFILES) $(HFILES)
		./Dosizes

xref:		,xref

,xref:		,tags
		./Doxref

tags:		,tags

,tags:		$(CFILES) $(HFILES)
		ctags $(CFILES) $(HFILES)
		mv tags ,tags

depend:
		grep '^#include' *.c ../../h/*.h		\
		| sed	-e 's/</"..\/..\/h\//;s/>/"/'		\
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
addarg.o: ../../h/conf.h
addarg.o: ../../h/kernel.h
addarg.o: ../../h/proc.h
addarg.o: ../../h/shell.h
ascdate.o: ../../h/conf.h
ascdate.o: ../../h/kernel.h
ascdate.o: ../../h/date.h
lexan.o: ../../h/conf.h
lexan.o: ../../h/kernel.h
lexan.o: ../../h/shell.h
login.o: ../../h/conf.h
login.o: ../../h/kernel.h
login.o: ../../h/shell.h
shell.o: ../../h/conf.h
shell.o: ../../h/kernel.h
shell.o: ../../h/proc.h
shell.o: ../../h/shell.h
shell.o: ../../h/cmd.h
shell.o: ../../h/tty.h
x_bpool.o: ../../h/conf.h
x_bpool.o: ../../h/kernel.h
x_bpool.o: ../../h/mark.h
x_bpool.o: ../../h/bufpool.h
x_cat.o: ../../h/conf.h
x_cat.o: ../../h/kernel.h
x_close.o: ../../h/conf.h
x_close.o: ../../h/kernel.h
x_cp.o: ../../h/conf.h
x_cp.o: ../../h/kernel.h
x_create.o: ../../h/conf.h
x_create.o: ../../h/kernel.h
x_create.o: ../../h/a.out.h
x_date.o: ../../h/conf.h
x_date.o: ../../h/kernel.h
x_date.o: ../../h/sleep.h
x_date.o: ../../h/date.h
x_devs.o: ../../h/conf.h
x_devs.o: ../../h/kernel.h
x_dg.o: ../../h/conf.h
x_dg.o: ../../h/kernel.h
x_dg.o: ../../h/network.h
x_echo.o: ../../h/conf.h
x_echo.o: ../../h/kernel.h
x_exit.o: ../../h/conf.h
x_exit.o: ../../h/kernel.h
x_exit.o: ../../h/shell.h
x_help.o: ../../h/conf.h
x_help.o: ../../h/kernel.h
x_help.o: ../../h/shell.h
x_help.o: ../../h/cmd.h
x_kill.o: ../../h/conf.h
x_kill.o: ../../h/kernel.h
x_kill.o: ../../h/shell.h
x_mem.o: ../../h/conf.h
x_mem.o: ../../h/kernel.h
x_mem.o: ../../h/proc.h
x_mem.o: ../../h/mem.h
x_mount.o: ../../h/conf.h
x_mount.o: ../../h/kernel.h
x_mount.o: ../../h/io.h
x_mount.o: ../../h/name.h
x_mv.o: ../../h/conf.h
x_mv.o: ../../h/kernel.h
x_mv.o: ../../h/file.h
x_net.o: ../../h/conf.h
x_net.o: ../../h/kernel.h
x_net.o: ../../h/network.h
x_ps.o: ../../h/conf.h
x_ps.o: ../../h/kernel.h
x_ps.o: ../../h/proc.h
x_reboot.o: ../../h/conf.h
x_reboot.o: ../../h/kernel.h
x_rf.o: ../../h/conf.h
x_rf.o: ../../h/kernel.h
x_rf.o: ../../h/fserver.h
x_rf.o: ../../h/rfile.h
x_rls.o: ../../h/conf.h
x_rls.o: ../../h/kernel.h
x_rls.o: ../../h/shell.h
x_rm.o: ../../h/conf.h
x_rm.o: ../../h/kernel.h
x_rm.o: ../../h/file.h
x_routes.o: ../../h/conf.h
x_routes.o: ../../h/kernel.h
x_routes.o: ../../h/proc.h
x_routes.o: ../../h/network.h
x_sleep.o: ../../h/conf.h
x_sleep.o: ../../h/kernel.h
x_snap.o: ../../h/conf.h
x_snap.o: ../../h/kernel.h
x_snap.o: ../../h/core11.h
x_unmou.o: ../../h/conf.h
x_unmou.o: ../../h/kernel.h
x_unmou.o: ../../h/io.h
x_unmou.o: ../../h/name.h
x_who.o: ../../h/conf.h
x_who.o: ../../h/kernel.h
x_who.o: ../../h/date.h
x_who.o: ../../h/shell.h
../../h/disk.h: ../../h/xebec.h
../../h/disk.h: ../../h/dtc.h
../../h/lfile.h: ../../h/iblock.h
../../h/lfile.h: ../../h/file.h
../../h/network.h: ../../h/deqna.h
../../h/network.h: ../../h/ether.h
../../h/network.h: ../../h/ip.h
../../h/network.h: ../../h/icmp.h
../../h/network.h: ../../h/udp.h
../../h/network.h: ../../h/net.h
../../h/network.h: ../../h/dgram.h
../../h/network.h: ../../h/arp.h
../../h/network.h: ../../h/fserver.h
../../h/network.h: ../../h/rfile.h
../../h/network.h: ../../h/domain.h
../../h/rfile.h: ../../h/file.h
../../h/shell.h: ../../h/mark.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
