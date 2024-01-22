#
#  Makefile for VM Xinu system routines (heart of the system)
#

# Directory where we are building Xinu
DIR	=	../../..

# Machine specific things we must define
INSTR_TYPE	=	RISC
ARCH		=	SPARC
MODEL		=	SPARC_SLC
CODE_NAME	=	sun4c


CDEFS	=	-D$(INSTR_TYPE) -D$(ARCH) -D$(MODEL) -D$(CODE_NAME) -DKERNEL
CFLAGS	=	-g -c -I../h ${CDEFS}
#CFLAGS	=	-O -c -I../h ${CDEFS}
SFLAGS	=	-I../h
ASCPPFLAGS =	-I../h -DASM ${CDEFS}
AR	=	/bin/ar
CC	=	/bin/cc
LD	=	/bin/ld
LIB	=	${DIR}/lib
HDIR	=	../h
MAKETD	= 	../../tools/maketd

OLIB	=	xsys.o

AFILE	=	../libx.a
TSTAMP	=	.last_make_all_run

DIR1	=	archdep
DIR2	=	archindep

DEPOFIL	= 	archdep/ctxsw.o		\
		archdep/freestk.o	\
		archdep/getdmem.o	\
		archdep/getidprom.o	\
		archdep/getstk.o	\
		archdep/hcreate.o	\
		archdep/intr.o		\
		archdep/kill.o		\
		archdep/kprintf.o	\
		archdep/lcreate.o	\
		archdep/mkdpool.o	\
		archdep/panic.o		\
		archdep/resched.o	\
		archdep/ret_mon.o	\
	 	archdep/evec.o		\
		archdep/#
#  Makefile for VM Xinu system routines (heart of the system)
#

# Directory where we are building Xinu
DIR	=	../../..

# Machine specific things we must define
INSTR_TYPE	=	RISC
ARCH		=	SPARC
MODEL		=	SPARC_SLC
CODE_NAME	=	sun4c


CDEFS	=	-D$(INSTR_TYPE) -D$(ARCH) -D$(MODEL) -D$(CODE_NAME) -DKERNEL
CFLAGS	=	-g -c -I../h ${CDEFS}
#CFLAGS	=	-O -c -I../h ${CDEFS}
SFLAGS	=	-I../h
ASCPPFLAGS =	-I../h -DASM ${CDEFS}
AR	=	/bin/ar
CC	=	/bin/cc
LD	=	/bin/ld
LIB	=	${DIR}/lib
HDIR	=	../h
MAKETD	= 	../../tools/maketd

OLIB	=	xsys.o

AFILE	=	../libx.a
TSTAMP	=	.last_make_all_run

DIR1	=	archdep
DIR2	=	archindep

DEPOFIL	= 	archdep/ctxsw.o		\
		archdep/freestk.o	\
		archdep/getdmem.o	\
		archdep/getidprom.o	\
		archdep/getstk.o	\
		archdep/hcreate.o	\
		archdep/intr.o		\
		archdep/kill.o		\
		archdep/kprintf.o	\
		archdep/lcreate.o	\
		archdep/mkdpool.o	\
		archdep/panic.o		\
		archdep/resched.o	\
		archdep/ret_mon.o	\
	 	archdep/evec.o		\
		archdep/configsys.o	\
		archdep/user_genericint.o	\
		archdep/kern_genericint.o	\
		archdep/int_demux.o	\
		archdep/proc_start.o	\
		archdep/regwin.o	\
		archdep/winerrs.o	\
		archdep/user_winint.o	\
		archdep/kern_winint.o	

INDEPOFIL = 	archindep/blkcopy.o	\
		archindep/blkequ.o	\
		archindep/blkcmp.o	\
		archindep/chprio.o	\
		archindep/close.o	\
		archindep/control.o	\
		archindep/fblkcopy.o	\
		archindep/freebuf.o	\
		archindep/freemem.o	\
		archindep/getbuf.o	\
		archindep/getc.o	\
		archindep/getitem.o	\
		archindep/getmem.o	\
		archindep/getpid.o	\
		archindep/getprio.o	\
		archindep/init.o	\
		archindep/insert.o	\
		archindep/insertd.o	\
		archindep/ioerr.o	\
		archindep/ionull.o	\
		archindep/mark.o	\
		archindep/mkpool.o	\
		archindep/mount.o	\
		archindep/newqueue.o	\
		archindep/nqalloc.o	\
		archindep/open.o	\
		archindep/pcount.o	\
		archindep/pcreate.o	\
		archindep/pdelete.o	\
		archindep/pinit.o	\
		archindep/poolinit.o	\
		archindep/preceive.o	\
		archindep/preset.o	\
		archindep/psend.o	\
		archindep/ptclear.o	\
		archindep/putc.o	\
		archindep/queue.o	\
		archindep/read.o	\
		archindep/ready.o	\
		archindep/receive.o	\
		archindep/recvclr.o	\
		archindep/recvtim.o	\
		archindep/resume.o	\
		archindep/scount.o	\
		archindep/screate.o	\
		archindep/sdelete.o	\
		archindep/seek.o	\
		archindep/send.o	\
		archindep/sendf.o	\
		archindep/setdev.o	\
		archindep/setnok.o	\
		archindep/signal.o	\
		archindep/signaln.o	\
		archindep/sleep.o	\
		archindep/sleep10.o	\
		archindep/sreset.o	\
		archindep/ssclock.o	\
		archindep/suspend.o	\
		archindep/unmount.o	\
		archindep/unsleep.o	\
		archindep/wait.o	\
		archindep/wakeup.o	\
		archindep/write.o	\
		archindep/xdone.o	\
		archindep/userret.o	\
		archindep/dvlookup.o	\
		archindep/sysopen.o	\
		archindep/getasid.o	\
		archindep/gpq.o		\
		archindep/gcache.o	\
		archindep/gettime.o	

ALLOFIL	=	${DEPOFIL} ${INDEPOFIL} ${INITOFIL}

.c.o:
	${CC} ${CFLAGS} -o $@ $<
.s.o:
	/lib/cpp ${ASCPPFLAGS} $< > .tmp.s
	${CC} -c ${SFLAGS} -o $@ .tmp.s

all:	${TSTAMP}

${TSTAMP}: ${ALLOFIL}
#	$(AR) cr ${AFILE} ${ALLOFIL}
	ld -r ${ALLOFIL} -o xsys.o
	touch ${TSTAMP}

top:
	cd .. ; make

archdep: ${DEPOFIL}
	$(AR) cr ${AFILE} ${DEPOFIL}

archindep: ${INDEPOFIL}
	$(AR) cr ${AFILE} ${INDEPOFIL}

init: ${INITOFIL}
	$(AR) cr ${AFILE} ${INITOFIL}

altall:	${OLIB}

${OLIB}: ${ALLOFIL}
	$(LD) -r ${ALLOFIL} -o ${OLIB}

alttop:
	cd .. ; make altall

clean:
	rm -rf ${ALLOFIL} ${OLIB}

depend:
	${MAKETD} -d -o${DIR1} ${CDEFS} -I${HDIR} ${DIR1}/*.[cs] > makedep
	${MAKETD} -d -o${DIR2} ${CDEFS} -I${HDIR} ${DIR2}/*.[cs] >> makedep
	echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
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


# DO NOT DELETE THIS LINE - maketd DEPENDS ON IT
# Dependencies generated at: Tue May  8 15:34:19 EST 1990
archdep/configsys.o: ../h/conf.h
archdep/configsys.o: ../h/frame.h
archdep/configsys.o: ../h/idprom.h
archdep/configsys.o: ../h/kernel.h
archdep/configsys.o: ../h/kernel.sparc.h
archdep/configsys.o: ../h/mem.h
archdep/configsys.o: ../h/openprom.h
archdep/configsys.o: ../h/types.h
archdep/configsys.o: ../h/vmem.h
archdep/configsys.o: ../h/vmem.sparc.h
archdep/configsys.o: archdep/configsys.c
archdep/ctxsw.o: ../h/asl.h
archdep/ctxsw.o: archdep/ctxsw.s
archdep/evec.o: ../h/a.out.h
archdep/evec.o: ../h/conf.h
archdep/evec.o: ../h/interreg.h
archdep/evec.o: ../h/io.h
archdep/evec.o: ../h/kernel.h
archdep/evec.o: ../h/kernel.sparc.h
archdep/evec.o: ../h/mem.h
archdep/evec.o: ../h/proc.h
archdep/evec.o: ../h/q.h
archdep/evec.o: ../h/select.h
archdep/evec.o: ../h/sem.h
archdep/evec.o: ../h/tty.h
archdep/evec.o: ../h/types.h
archdep/evec.o: ../h/vmem.h
archdep/evec.o: ../h/vmem.sparc.h
archdep/evec.o: archdep/evec.c
archdep/freestk.o: ../h/a.out.h
archdep/freestk.o: ../h/conf.h
archdep/freestk.o: ../h/frame.h
archdep/freestk.o: ../h/kernel.h
archdep/freestk.o: ../h/kernel.sparc.h
archdep/freestk.o: ../h/mem.h
archdep/freestk.o: ../h/proc.h
archdep/freestk.o: ../h/select.h
archdep/freestk.o: ../h/types.h
archdep/freestk.o: ../h/vmem.h
archdep/freestk.o: ../h/vmem.sparc.h
archdep/freestk.o: archdep/freestk.c
archdep/getdmem.o: ../h/a.out.h
archdep/getdmem.o: ../h/arp.h
archdep/getdmem.o: ../h/conf.h
archdep/getdmem.o: ../h/daemon.h
archdep/getdmem.o: ../h/dgram.h
archdep/getdmem.o: ../h/domain.h
archdep/getdmem.o: ../h/ether.h
archdep/getdmem.o: ../h/file.h
archdep/getdmem.o: ../h/fserver.h
archdep/getdmem.o: ../h/icmp.h
archdep/getdmem.o: ../h/ip.h
archdep/getdmem.o: ../h/ipreass.h
archdep/getdmem.o: ../h/kernel.h
archdep/getdmem.o: ../h/kernel.sparc.h
archdep/getdmem.o: ../h/mark.h
archdep/getdmem.o: ../h/mem.h
archdep/getdmem.o: ../h/net.h
archdep/getdmem.o: ../h/netconf.h
archdep/getdmem.o: ../h/netif.h
archdep/getdmem.o: ../h/network.h
archdep/getdmem.o: ../h/proc.h
archdep/getdmem.o: ../h/rfile.h
archdep/getdmem.o: ../h/rip.h
archdep/getdmem.o: ../h/route.h
archdep/getdmem.o: ../h/select.h
archdep/getdmem.o: ../h/snmpvars.h
archdep/getdmem.o: ../h/types.h
archdep/getdmem.o: ../h/udp.h
archdep/getdmem.o: ../h/vmem.h
archdep/getdmem.o: ../h/vmem.sparc.h
archdep/getdmem.o: archdep/getdmem.c
archdep/getidprom.o: ../h/asl.h
archdep/getidprom.o: ../h/eeprom.h
archdep/getidprom.o: archdep/getidprom.s
archdep/getstk.o: ../h/a.out.h
archdep/getstk.o: ../h/conf.h
archdep/getstk.o: ../h/kernel.h
archdep/getstk.o: ../h/kernel.sparc.h
archdep/getstk.o: ../h/proc.h
archdep/getstk.o: ../h/select.h
archdep/getstk.o: ../h/types.h
archdep/getstk.o: ../h/vmem.h
archdep/getstk.o: ../h/vmem.sparc.h
archdep/getstk.o: archdep/getstk.c
archdep/hcreate.o: ../h/a.out.h
archdep/hcreate.o: ../h/conf.h
archdep/hcreate.o: ../h/kernel.h
archdep/hcreate.o: ../h/kernel.sparc.h
archdep/hcreate.o: ../h/mem.h
archdep/hcreate.o: ../h/proc.h
archdep/hcreate.o: ../h/select.h
archdep/hcreate.o: ../h/types.h
archdep/hcreate.o: ../h/vmem.h
archdep/hcreate.o: ../h/vmem.sparc.h
archdep/hcreate.o: archdep/hcreate.c
archdep/initialize.o: ../h/a.out.h
archdep/initialize.o: ../h/arp.h
archdep/initialize.o: ../h/conf.h
archdep/initialize.o: ../h/daemon.h
archdep/initialize.o: ../h/dgram.h
archdep/initialize.o: ../h/domain.h
archdep/initialize.o: ../h/ether.h
archdep/initialize.o: ../h/file.h
archdep/initialize.o: ../h/frame.h
archdep/initialize.o: ../h/fserver.h
archdep/initialize.o: ../h/icmp.h
archdep/initialize.o: ../h/io.h
archdep/initialize.o: ../h/ip.h
archdep/initialize.o: ../h/ipreass.h
archdep/initialize.o: ../h/kernel.h
archdep/initialize.o: ../h/kernel.sparc.h
archdep/initialize.o: ../h/mem.h
archdep/initialize.o: ../h/net.h
archdep/initialize.o: ../h/netconf.h
archdep/initialize.o: ../h/netif.h
archdep/initialize.o: ../h/network.h
archdep/initialize.o: ../h/proc.h
archdep/initialize.o: ../h/q.h
archdep/initialize.o: ../h/rfile.h
archdep/initialize.o: ../h/rip.h
archdep/initialize.o: ../h/route.h
archdep/initialize.o: ../h/select.h
archdep/initialize.o: ../h/sem.h
archdep/initialize.o: ../h/sleep.h
archdep/initialize.o: ../h/snmpvars.h
archdep/initialize.o: ../h/tty.h
archdep/initialize.o: ../h/types.h
archdep/initialize.o: ../h/udp.h
archdep/initialize.o: ../h/vmem.h
archdep/initialize.o: ../h/vmem.sparc.h
archdep/initialize.o: archdep/initialize.c
archdep/int_demux.o: ../h/traps.h
archdep/int_demux.o: archdep/int_demux.c
archdep/intr.o: ../h/asl.h
archdep/intr.o: ../h/psl.h
archdep/intr.o: ../h/user_traps.h
archdep/intr.o: ../h/vmem.h
archdep/intr.o: ../h/vmem.sparc.h
archdep/intr.o: archdep/intr.s
archdep/k_traptable.o: archdep/k_traptable.s
archdep/kern_genericint.o: ../h/asl.h
archdep/kern_genericint.o: ../h/psl.h
archdep/kern_genericint.o: ../h/vmem.h
archdep/kern_genericint.o: ../h/vmem.sparc.h
archdep/kern_genericint.o: archdep/kern_genericint.s
archdep/kern_winint.o: ../h/asl.h
archdep/kern_winint.o: ../h/psl.h
archdep/kern_winint.o: ../h/reg.h
archdep/kern_winint.o: ../h/vmem.h
archdep/kern_winint.o: ../h/vmem.sparc.h
archdep/kern_winint.o: archdep/kern_winint.s
archdep/kill.o: ../h/a.out.h
archdep/kill.o: ../h/conf.h
archdep/kill.o: ../h/io.h
archdep/kill.o: ../h/kernel.h
archdep/kill.o: ../h/kernel.sparc.h
archdep/kill.o: ../h/proc.h
archdep/kill.o: ../h/q.h
archdep/kill.o: ../h/select.h
archdep/kill.o: ../h/sem.h
archdep/kill.o: ../h/types.h
archdep/kill.o: ../h/vmem.h
archdep/kill.o: ../h/vmem.sparc.h
archdep/kill.o: ../h/xpppgio.h
archdep/kill.o: archdep/kill.c
archdep/kprintf.o: ../h/conf.h
archdep/kprintf.o: ../h/io.h
archdep/kprintf.o: ../h/kernel.h
archdep/kprintf.o: ../h/kernel.sparc.h
archdep/kprintf.o: ../h/openprom.h
archdep/kprintf.o: ../h/tty.h
archdep/kprintf.o: ../h/types.h
archdep/kprintf.o: archdep/kprintf.c
archdep/lcreate.o: ../h/a.out.h
archdep/lcreate.o: ../h/arp.h
archdep/lcreate.o: ../h/asl.h
archdep/lcreate.o: ../h/conf.h
archdep/lcreate.o: ../h/daemon.h
archdep/lcreate.o: ../h/dgram.h
archdep/lcreate.o: ../h/domain.h
archdep/lcreate.o: ../h/ether.h
archdep/lcreate.o: ../h/file.h
archdep/lcreate.o: ../h/frame.h
archdep/lcreate.o: ../h/fserver.h
archdep/lcreate.o: ../h/icmp.h
archdep/lcreate.o: ../h/io.h
archdep/lcreate.o: ../h/ip.h
archdep/lcreate.o: ../h/ipreass.h
archdep/lcreate.o: ../h/kernel.h
archdep/lcreate.o: ../h/kernel.sparc.h
archdep/lcreate.o: ../h/mem.h
archdep/lcreate.o: ../h/name.h
archdep/lcreate.o: ../h/net.h
archdep/lcreate.o: ../h/netconf.h
archdep/lcreate.o: ../h/netif.h
archdep/lcreate.o: ../h/network.h
archdep/lcreate.o: ../h/proc.h
archdep/lcreate.o: ../h/q.h
archdep/lcreate.o: ../h/rfile.h
archdep/lcreate.o: ../h/rip.h
archdep/lcreate.o: ../h/route.h
archdep/lcreate.o: ../h/select.h
archdep/lcreate.o: ../h/snmpvars.h
archdep/lcreate.o: ../h/types.h
archdep/lcreate.o: ../h/udp.h
archdep/lcreate.o: ../h/vmem.h
archdep/lcreate.o: ../h/vmem.sparc.h
archdep/lcreate.o: ../h/xpppgio.h
archdep/lcreate.o: archdep/lcreate.c
archdep/mkdpool.o: ../h/arp.h
archdep/mkdpool.o: ../h/bufpool.h
archdep/mkdpool.o: ../h/conf.h
archdep/mkdpool.o: ../h/daemon.h
archdep/mkdpool.o: ../h/dgram.h
archdep/mkdpool.o: ../h/domain.h
archdep/mkdpool.o: ../h/ether.h
archdep/mkdpool.o: ../h/file.h
archdep/mkdpool.o: ../h/fserver.h
archdep/mkdpool.o: ../h/icmp.h
archdep/mkdpool.o: ../h/ip.h
archdep/mkdpool.o: ../h/ipreass.h
archdep/mkdpool.o: ../h/kernel.h
archdep/mkdpool.o: ../h/kernel.sparc.h
archdep/mkdpool.o: ../h/mark.h
archdep/mkdpool.o: ../h/mem.h
archdep/mkdpool.o: ../h/net.h
archdep/mkdpool.o: ../h/netconf.h
archdep/mkdpool.o: ../h/netif.h
archdep/mkdpool.o: ../h/network.h
archdep/mkdpool.o: ../h/rfile.h
archdep/mkdpool.o: ../h/rip.h
archdep/mkdpool.o: ../h/route.h
archdep/mkdpool.o: ../h/snmpvars.h
archdep/mkdpool.o: ../h/types.h
archdep/mkdpool.o: ../h/udp.h
archdep/mkdpool.o: archdep/mkdpool.c
archdep/panic.o: ../h/a.out.h
archdep/panic.o: ../h/conf.h
archdep/panic.o: ../h/kernel.h
archdep/panic.o: ../h/kernel.sparc.h
archdep/panic.o: ../h/proc.h
archdep/panic.o: ../h/select.h
archdep/panic.o: ../h/types.h
archdep/panic.o: archdep/panic.c
archdep/proc_start.o: ../h/asl-machine-dependent.h
archdep/proc_start.o: ../h/asl.h
archdep/proc_start.o: ../h/vmem.h
archdep/proc_start.o: ../h/vmem.sparc.h
archdep/proc_start.o: archdep/proc_start.s
archdep/regwin.o: ../h/asl.h
archdep/regwin.o: ../h/reg.h
archdep/regwin.o: archdep/regwin.s
archdep/resched.o: ../h/a.out.h
archdep/resched.o: ../h/conf.h
archdep/resched.o: ../h/kernel.h
archdep/resched.o: ../h/kernel.sparc.h
archdep/resched.o: ../h/proc.h
archdep/resched.o: ../h/q.h
archdep/resched.o: ../h/select.h
archdep/resched.o: ../h/types.h
archdep/resched.o: ../h/vmem.h
archdep/resched.o: ../h/vmem.sparc.h
archdep/resched.o: archdep/resched.c
archdep/ret_mon.o: ../h/conf.h
archdep/ret_mon.o: ../h/kernel.h
archdep/ret_mon.o: ../h/kernel.sparc.h
archdep/ret_mon.o: ../h/openprom.h
archdep/ret_mon.o: ../h/types.h
archdep/ret_mon.o: ../h/vmem.h
archdep/ret_mon.o: ../h/vmem.sparc.h
archdep/ret_mon.o: archdep/ret_mon.c
archdep/startup.o: ../sys/archdep/k_traptable.s
archdep/startup.o: ../sys/archdep/u_traptable.s
archdep/startup.o: ../h/asl-machine-dependent.h
archdep/startup.o: ../h/asl.h
archdep/startup.o: ../h/psl.h
archdep/startup.o: archdep/startup.s
archdep/u_traptable.o: archdep/u_traptable.s
archdep/user_genericint.o: ../h/asl.h
archdep/user_genericint.o: ../h/psl.h
archdep/user_genericint.o: ../h/vmem.h
archdep/user_genericint.o: ../h/vmem.sparc.h
archdep/user_genericint.o: archdep/user_genericint.s
archdep/user_winint.o: ../h/asl.h
archdep/user_winint.o: ../h/reg.h
archdep/user_winint.o: ../h/vmem.h
archdep/user_winint.o: ../h/vmem.sparc.h
archdep/user_winint.o: archdep/user_winint.s
archdep/winerrs.o: ../h/a.out.h
archdep/winerrs.o: ../h/kernel.h
archdep/winerrs.o: ../h/kernel.sparc.h
archdep/winerrs.o: ../h/proc.h
archdep/winerrs.o: ../h/traps.h
archdep/winerrs.o: ../h/types.h
archdep/winerrs.o: archdep/winerrs.c
archindep/blkcmp.o: ../h/conf.h
archindep/blkcmp.o: ../h/kernel.h
archindep/blkcmp.o: ../h/kernel.sparc.h
archindep/blkcmp.o: ../h/types.h
archindep/blkcmp.o: archindep/blkcmp.c
archindep/blkcopy.o: ../h/kernel.h
archindep/blkcopy.o: ../h/kernel.sparc.h
archindep/blkcopy.o: ../h/types.h
archindep/blkcopy.o: archindep/blkcopy.c
archindep/blkequ.o: ../h/kernel.h
archindep/blkequ.o: ../h/kernel.sparc.h
archindep/blkequ.o: ../h/types.h
archindep/blkequ.o: archindep/blkequ.c
archindep/chprio.o: ../h/a.out.h
archindep/chprio.o: ../h/conf.h
archindep/chprio.o: ../h/kernel.h
archindep/chprio.o: ../h/kernel.sparc.h
archindep/chprio.o: ../h/proc.h
archindep/chprio.o: ../h/select.h
archindep/chprio.o: ../h/types.h
archindep/chprio.o: archindep/chprio.c
archindep/close.o: ../h/conf.h
archindep/close.o: ../h/io.h
archindep/close.o: ../h/kernel.h
archindep/close.o: ../h/kernel.sparc.h
archindep/close.o: ../h/types.h
archindep/close.o: archindep/close.c
archindep/control.o: ../h/conf.h
archindep/control.o: ../h/io.h
archindep/control.o: ../h/kernel.h
archindep/control.o: ../h/kernel.sparc.h
archindep/control.o: ../h/types.h
archindep/control.o: archindep/control.c
archindep/dvlookup.o: ../h/conf.h
archindep/dvlookup.o: ../h/kernel.h
archindep/dvlookup.o: ../h/kernel.sparc.h
archindep/dvlookup.o: ../h/types.h
archindep/dvlookup.o: archindep/dvlookup.c
archindep/fblkcopy.o: ../h/kernel.h
archindep/fblkcopy.o: ../h/kernel.sparc.h
archindep/fblkcopy.o: ../h/types.h
archindep/fblkcopy.o: archindep/fblkcopy.c
archindep/freebuf.o: ../h/bufpool.h
archindep/freebuf.o: ../h/conf.h
archindep/freebuf.o: ../h/kernel.h
archindep/freebuf.o: ../h/kernel.sparc.h
archindep/freebuf.o: ../h/mark.h
archindep/freebuf.o: ../h/types.h
archindep/freebuf.o: archindep/freebuf.c
archindep/freemem.o: ../h/conf.h
archindep/freemem.o: ../h/kernel.h
archindep/freemem.o: ../h/kernel.sparc.h
archindep/freemem.o: ../h/mem.h
archindep/freemem.o: ../h/types.h
archindep/freemem.o: ../h/vmem.h
archindep/freemem.o: ../h/vmem.sparc.h
archindep/freemem.o: archindep/freemem.c
archindep/gcache.o: ../h/conf.h
archindep/gcache.o: ../h/gcache.h
archindep/gcache.o: ../h/kernel.h
archindep/gcache.o: ../h/kernel.sparc.h
archindep/gcache.o: ../h/types.h
archindep/gcache.o: archindep/gcache.c
archindep/getasid.o: ../h/a.out.h
archindep/getasid.o: ../h/conf.h
archindep/getasid.o: ../h/kernel.h
archindep/getasid.o: ../h/kernel.sparc.h
archindep/getasid.o: ../h/proc.h
archindep/getasid.o: ../h/select.h
archindep/getasid.o: ../h/types.h
archindep/getasid.o: archindep/getasid.c
archindep/getbuf.o: ../h/bufpool.h
archindep/getbuf.o: ../h/conf.h
archindep/getbuf.o: ../h/kernel.h
archindep/getbuf.o: ../h/kernel.sparc.h
archindep/getbuf.o: ../h/mark.h
archindep/getbuf.o: ../h/types.h
archindep/getbuf.o: archindep/getbuf.c
archindep/getc.o: ../h/conf.h
archindep/getc.o: ../h/io.h
archindep/getc.o: ../h/kernel.h
archindep/getc.o: ../h/kernel.sparc.h
archindep/getc.o: ../h/types.h
archindep/getc.o: archindep/getc.c
archindep/getitem.o: ../h/conf.h
archindep/getitem.o: ../h/kernel.h
archindep/getitem.o: ../h/kernel.sparc.h
archindep/getitem.o: ../h/q.h
archindep/getitem.o: ../h/types.h
archindep/getitem.o: archindep/getitem.c
archindep/getmem.o: ../h/conf.h
archindep/getmem.o: ../h/kernel.h
archindep/getmem.o: ../h/kernel.sparc.h
archindep/getmem.o: ../h/mem.h
archindep/getmem.o: ../h/types.h
archindep/getmem.o: archindep/getmem.c
archindep/getpid.o: ../h/a.out.h
archindep/getpid.o: ../h/conf.h
archindep/getpid.o: ../h/kernel.h
archindep/getpid.o: ../h/kernel.sparc.h
archindep/getpid.o: ../h/proc.h
archindep/getpid.o: ../h/select.h
archindep/getpid.o: ../h/types.h
archindep/getpid.o: archindep/getpid.c
archindep/getprio.o: ../h/a.out.h
archindep/getprio.o: ../h/conf.h
archindep/getprio.o: ../h/kernel.h
archindep/getprio.o: ../h/kernel.sparc.h
archindep/getprio.o: ../h/proc.h
archindep/getprio.o: ../h/select.h
archindep/getprio.o: ../h/types.h
archindep/getprio.o: archindep/getprio.c
archindep/gettime.o: ../h/clock.h
archindep/gettime.o: ../h/conf.h
archindep/gettime.o: ../h/date.h
archindep/gettime.o: ../h/kernel.h
archindep/gettime.o: ../h/kernel.sparc.h
archindep/gettime.o: ../h/sleep.h
archindep/gettime.o: ../h/types.h
archindep/gettime.o: archindep/gettime.c
archindep/gpq.o: ../h/conf.h
archindep/gpq.o: ../h/kernel.h
archindep/gpq.o: ../h/kernel.sparc.h
archindep/gpq.o: ../h/mark.h
archindep/gpq.o: ../h/q.h
archindep/gpq.o: ../h/types.h
archindep/gpq.o: archindep/gpq.c
archindep/init.o: ../h/conf.h
archindep/init.o: ../h/io.h
archindep/init.o: ../h/kernel.h
archindep/init.o: ../h/kernel.sparc.h
archindep/init.o: ../h/types.h
archindep/init.o: archindep/init.c
archindep/insert.o: ../h/conf.h
archindep/insert.o: ../h/kernel.h
archindep/insert.o: ../h/kernel.sparc.h
archindep/insert.o: ../h/q.h
archindep/insert.o: ../h/types.h
archindep/insert.o: archindep/insert.c
archindep/insertd.o: ../h/conf.h
archindep/insertd.o: ../h/kernel.h
archindep/insertd.o: ../h/kernel.sparc.h
archindep/insertd.o: ../h/q.h
archindep/insertd.o: ../h/types.h
archindep/insertd.o: archindep/insertd.c
archindep/ioerr.o: ../h/conf.h
archindep/ioerr.o: ../h/kernel.h
archindep/ioerr.o: ../h/kernel.sparc.h
archindep/ioerr.o: ../h/types.h
archindep/ioerr.o: archindep/ioerr.c
archindep/ionull.o: ../h/conf.h
archindep/ionull.o: ../h/kernel.h
archindep/ionull.o: ../h/kernel.sparc.h
archindep/ionull.o: ../h/types.h
archindep/ionull.o: archindep/ionull.c
archindep/mark.o: ../h/conf.h
archindep/mark.o: ../h/kernel.h
archindep/mark.o: ../h/kernel.sparc.h
archindep/mark.o: ../h/mark.h
archindep/mark.o: ../h/types.h
archindep/mark.o: archindep/mark.c
archindep/mkpool.o: ../h/bufpool.h
archindep/mkpool.o: ../h/conf.h
archindep/mkpool.o: ../h/kernel.h
archindep/mkpool.o: ../h/kernel.sparc.h
archindep/mkpool.o: ../h/mark.h
archindep/mkpool.o: ../h/types.h
archindep/mkpool.o: archindep/mkpool.c
archindep/mount.o: ../h/conf.h
archindep/mount.o: ../h/kernel.h
archindep/mount.o: ../h/kernel.sparc.h
archindep/mount.o: ../h/name.h
archindep/mount.o: ../h/types.h
archindep/mount.o: archindep/mount.c
archindep/newqueue.o: ../h/conf.h
archindep/newqueue.o: ../h/kernel.h
archindep/newqueue.o: ../h/kernel.sparc.h
archindep/newqueue.o: ../h/q.h
archindep/newqueue.o: ../h/types.h
archindep/newqueue.o: archindep/newqueue.c
archindep/nqalloc.o: ../h/a.out.h
archindep/nqalloc.o: ../h/arp.h
archindep/nqalloc.o: ../h/conf.h
archindep/nqalloc.o: ../h/daemon.h
archindep/nqalloc.o: ../h/dgram.h
archindep/nqalloc.o: ../h/domain.h
archindep/nqalloc.o: ../h/ether.h
archindep/nqalloc.o: ../h/file.h
archindep/nqalloc.o: ../h/fserver.h
archindep/nqalloc.o: ../h/icmp.h
archindep/nqalloc.o: ../h/ip.h
archindep/nqalloc.o: ../h/ipreass.h
archindep/nqalloc.o: ../h/kernel.h
archindep/nqalloc.o: ../h/kernel.sparc.h
archindep/nqalloc.o: ../h/net.h
archindep/nqalloc.o: ../h/netconf.h
archindep/nqalloc.o: ../h/netif.h
archindep/nqalloc.o: ../h/network.h
archindep/nqalloc.o: ../h/proc.h
archindep/nqalloc.o: ../h/rfile.h
archindep/nqalloc.o: ../h/rip.h
archindep/nqalloc.o: ../h/route.h
archindep/nqalloc.o: ../h/select.h
archindep/nqalloc.o: ../h/snmpvars.h
archindep/nqalloc.o: ../h/types.h
archindep/nqalloc.o: ../h/udp.h
archindep/nqalloc.o: archindep/nqalloc.c
archindep/open.o: ../h/conf.h
archindep/open.o: ../h/io.h
archindep/open.o: ../h/kernel.h
archindep/open.o: ../h/kernel.sparc.h
archindep/open.o: ../h/types.h
archindep/open.o: archindep/open.c
archindep/pcount.o: ../h/conf.h
archindep/pcount.o: ../h/kernel.h
archindep/pcount.o: ../h/kernel.sparc.h
archindep/pcount.o: ../h/mark.h
archindep/pcount.o: ../h/ports.h
archindep/pcount.o: ../h/types.h
archindep/pcount.o: archindep/pcount.c
archindep/pcreate.o: ../h/conf.h
archindep/pcreate.o: ../h/kernel.h
archindep/pcreate.o: ../h/kernel.sparc.h
archindep/pcreate.o: ../h/mark.h
archindep/pcreate.o: ../h/ports.h
archindep/pcreate.o: ../h/types.h
archindep/pcreate.o: archindep/pcreate.c
archindep/pdelete.o: ../h/conf.h
archindep/pdelete.o: ../h/kernel.h
archindep/pdelete.o: ../h/kernel.sparc.h
archindep/pdelete.o: ../h/mark.h
archindep/pdelete.o: ../h/ports.h
archindep/pdelete.o: ../h/types.h
archindep/pdelete.o: archindep/pdelete.c
archindep/pinit.o: ../h/conf.h
archindep/pinit.o: ../h/kernel.h
archindep/pinit.o: ../h/kernel.sparc.h
archindep/pinit.o: ../h/mark.h
archindep/pinit.o: ../h/ports.h
archindep/pinit.o: ../h/types.h
archindep/pinit.o: archindep/pinit.c
archindep/poolinit.o: ../h/bufpool.h
archindep/poolinit.o: ../h/conf.h
archindep/poolinit.o: ../h/kernel.h
archindep/poolinit.o: ../h/kernel.sparc.h
archindep/poolinit.o: ../h/mark.h
archindep/poolinit.o: ../h/types.h
archindep/poolinit.o: archindep/poolinit.c
archindep/preceive.o: ../h/conf.h
archindep/preceive.o: ../h/kernel.h
archindep/preceive.o: ../h/kernel.sparc.h
archindep/preceive.o: ../h/mark.h
archindep/preceive.o: ../h/ports.h
archindep/preceive.o: ../h/types.h
archindep/preceive.o: archindep/preceive.c
archindep/preset.o: ../h/conf.h
archindep/preset.o: ../h/kernel.h
archindep/preset.o: ../h/kernel.sparc.h
archindep/preset.o: ../h/mark.h
archindep/preset.o: ../h/ports.h
archindep/preset.o: ../h/types.h
archindep/preset.o: archindep/preset.c
archindep/psend.o: ../h/conf.h
archindep/psend.o: ../h/kernel.h
archindep/psend.o: ../h/kernel.sparc.h
archindep/psend.o: ../h/mark.h
archindep/psend.o: ../h/ports.h
archindep/psend.o: ../h/types.h
archindep/psend.o: archindep/psend.c
archindep/ptclear.o: ../h/conf.h
archindep/ptclear.o: ../h/kernel.h
archindep/ptclear.o: ../h/kernel.sparc.h
archindep/ptclear.o: ../h/mark.h
archindep/ptclear.o: ../h/ports.h
archindep/ptclear.o: ../h/types.h
archindep/ptclear.o: archindep/ptclear.c
archindep/putc.o: ../h/conf.h
archindep/putc.o: ../h/io.h
archindep/putc.o: ../h/kernel.h
archindep/putc.o: ../h/kernel.sparc.h
archindep/putc.o: ../h/types.h
archindep/putc.o: archindep/putc.c
archindep/queue.o: ../h/conf.h
archindep/queue.o: ../h/kernel.h
archindep/queue.o: ../h/kernel.sparc.h
archindep/queue.o: ../h/q.h
archindep/queue.o: ../h/types.h
archindep/queue.o: archindep/queue.c
archindep/read.o: ../h/conf.h
archindep/read.o: ../h/io.h
archindep/read.o: ../h/kernel.h
archindep/read.o: ../h/kernel.sparc.h
archindep/read.o: ../h/types.h
archindep/read.o: archindep/read.c
archindep/ready.o: ../h/a.out.h
archindep/ready.o: ../h/conf.h
archindep/ready.o: ../h/kernel.h
archindep/ready.o: ../h/kernel.sparc.h
archindep/ready.o: ../h/proc.h
archindep/ready.o: ../h/q.h
archindep/ready.o: ../h/select.h
archindep/ready.o: ../h/types.h
archindep/ready.o: archindep/ready.c
archindep/receive.o: ../h/a.out.h
archindep/receive.o: ../h/conf.h
archindep/receive.o: ../h/kernel.h
archindep/receive.o: ../h/kernel.sparc.h
archindep/receive.o: ../h/proc.h
archindep/receive.o: ../h/select.h
archindep/receive.o: ../h/types.h
archindep/receive.o: archindep/receive.c
archindep/recvclr.o: ../h/a.out.h
archindep/recvclr.o: ../h/conf.h
archindep/recvclr.o: ../h/kernel.h
archindep/recvclr.o: ../h/kernel.sparc.h
archindep/recvclr.o: ../h/proc.h
archindep/recvclr.o: ../h/select.h
archindep/recvclr.o: ../h/types.h
archindep/recvclr.o: archindep/recvclr.c
archindep/recvtim.o: ../h/a.out.h
archindep/recvtim.o: ../h/conf.h
archindep/recvtim.o: ../h/kernel.h
archindep/recvtim.o: ../h/kernel.sparc.h
archindep/recvtim.o: ../h/proc.h
archindep/recvtim.o: ../h/q.h
archindep/recvtim.o: ../h/select.h
archindep/recvtim.o: ../h/sleep.h
archindep/recvtim.o: ../h/types.h
archindep/recvtim.o: archindep/recvtim.c
archindep/resume.o: ../h/a.out.h
archindep/resume.o: ../h/conf.h
archindep/resume.o: ../h/kernel.h
archindep/resume.o: ../h/kernel.sparc.h
archindep/resume.o: ../h/proc.h
archindep/resume.o: ../h/select.h
archindep/resume.o: ../h/types.h
archindep/resume.o: archindep/resume.c
archindep/scount.o: ../h/conf.h
archindep/scount.o: ../h/kernel.h
archindep/scount.o: ../h/kernel.sparc.h
archindep/scount.o: ../h/sem.h
archindep/scount.o: ../h/types.h
archindep/scount.o: archindep/scount.c
archindep/screate.o: ../h/a.out.h
archindep/screate.o: ../h/conf.h
archindep/screate.o: ../h/kernel.h
archindep/screate.o: ../h/kernel.sparc.h
archindep/screate.o: ../h/proc.h
archindep/screate.o: ../h/q.h
archindep/screate.o: ../h/select.h
archindep/screate.o: ../h/sem.h
archindep/screate.o: ../h/types.h
archindep/screate.o: archindep/screate.c
archindep/sdelete.o: ../h/a.out.h
archindep/sdelete.o: ../h/conf.h
archindep/sdelete.o: ../h/kernel.h
archindep/sdelete.o: ../h/kernel.sparc.h
archindep/sdelete.o: ../h/proc.h
archindep/sdelete.o: ../h/q.h
archindep/sdelete.o: ../h/select.h
archindep/sdelete.o: ../h/sem.h
archindep/sdelete.o: ../h/types.h
archindep/sdelete.o: archindep/sdelete.c
archindep/seek.o: ../h/conf.h
archindep/seek.o: ../h/io.h
archindep/seek.o: ../h/kernel.h
archindep/seek.o: ../h/kernel.sparc.h
archindep/seek.o: ../h/types.h
archindep/seek.o: archindep/seek.c
archindep/send.o: ../h/a.out.h
archindep/send.o: ../h/conf.h
archindep/send.o: ../h/kernel.h
archindep/send.o: ../h/kernel.sparc.h
archindep/send.o: ../h/proc.h
archindep/send.o: ../h/select.h
archindep/send.o: ../h/types.h
archindep/send.o: archindep/send.c
archindep/sendf.o: ../h/a.out.h
archindep/sendf.o: ../h/conf.h
archindep/sendf.o: ../h/kernel.h
archindep/sendf.o: ../h/kernel.sparc.h
archindep/sendf.o: ../h/proc.h
archindep/sendf.o: ../h/select.h
archindep/sendf.o: ../h/types.h
archindep/sendf.o: archindep/sendf.c
archindep/setdev.o: ../h/a.out.h
archindep/setdev.o: ../h/conf.h
archindep/setdev.o: ../h/kernel.h
archindep/setdev.o: ../h/kernel.sparc.h
archindep/setdev.o: ../h/proc.h
archindep/setdev.o: ../h/select.h
archindep/setdev.o: ../h/types.h
archindep/setdev.o: archindep/setdev.c
archindep/setnok.o: ../h/a.out.h
archindep/setnok.o: ../h/conf.h
archindep/setnok.o: ../h/kernel.h
archindep/setnok.o: ../h/kernel.sparc.h
archindep/setnok.o: ../h/proc.h
archindep/setnok.o: ../h/select.h
archindep/setnok.o: ../h/types.h
archindep/setnok.o: archindep/setnok.c
archindep/signal.o: ../h/a.out.h
archindep/signal.o: ../h/conf.h
archindep/signal.o: ../h/kernel.h
archindep/signal.o: ../h/kernel.sparc.h
archindep/signal.o: ../h/proc.h
archindep/signal.o: ../h/q.h
archindep/signal.o: ../h/select.h
archindep/signal.o: ../h/sem.h
archindep/signal.o: ../h/types.h
archindep/signal.o: archindep/signal.c
archindep/signaln.o: ../h/a.out.h
archindep/signaln.o: ../h/conf.h
archindep/signaln.o: ../h/kernel.h
archindep/signaln.o: ../h/kernel.sparc.h
archindep/signaln.o: ../h/proc.h
archindep/signaln.o: ../h/q.h
archindep/signaln.o: ../h/select.h
archindep/signaln.o: ../h/sem.h
archindep/signaln.o: ../h/types.h
archindep/signaln.o: archindep/signaln.c
archindep/sleep.o: ../h/a.out.h
archindep/sleep.o: ../h/conf.h
archindep/sleep.o: ../h/kernel.h
archindep/sleep.o: ../h/kernel.sparc.h
archindep/sleep.o: ../h/proc.h
archindep/sleep.o: ../h/q.h
archindep/sleep.o: ../h/select.h
archindep/sleep.o: ../h/sleep.h
archindep/sleep.o: ../h/types.h
archindep/sleep.o: archindep/sleep.c
archindep/sleep10.o: ../h/a.out.h
archindep/sleep10.o: ../h/conf.h
archindep/sleep10.o: ../h/kernel.h
archindep/sleep10.o: ../h/kernel.sparc.h
archindep/sleep10.o: ../h/proc.h
archindep/sleep10.o: ../h/q.h
archindep/sleep10.o: ../h/select.h
archindep/sleep10.o: ../h/sleep.h
archindep/sleep10.o: ../h/types.h
archindep/sleep10.o: archindep/sleep10.c
archindep/sreset.o: ../h/a.out.h
archindep/sreset.o: ../h/conf.h
archindep/sreset.o: ../h/kernel.h
archindep/sreset.o: ../h/kernel.sparc.h
archindep/sreset.o: ../h/proc.h
archindep/sreset.o: ../h/q.h
archindep/sreset.o: ../h/select.h
archindep/sreset.o: ../h/sem.h
archindep/sreset.o: ../h/types.h
archindep/sreset.o: archindep/sreset.c
archindep/ssclock.o: ../h/a.out.h
archindep/ssclock.o: ../h/conf.h
archindep/ssclock.o: ../h/kernel.h
archindep/ssclock.o: ../h/kernel.sparc.h
archindep/ssclock.o: ../h/proc.h
archindep/ssclock.o: ../h/q.h
archindep/ssclock.o: ../h/select.h
archindep/ssclock.o: ../h/sleep.h
archindep/ssclock.o: ../h/types.h
archindep/ssclock.o: archindep/ssclock.c
archindep/suspend.o: ../h/a.out.h
archindep/suspend.o: ../h/conf.h
archindep/suspend.o: ../h/kernel.h
archindep/suspend.o: ../h/kernel.sparc.h
archindep/suspend.o: ../h/proc.h
archindep/suspend.o: ../h/select.h
archindep/suspend.o: ../h/types.h
archindep/suspend.o: archindep/suspend.c
archindep/sysopen.o: ../h/conf.h
archindep/sysopen.o: ../h/io.h
archindep/sysopen.o: ../h/kernel.h
archindep/sysopen.o: ../h/kernel.sparc.h
archindep/sysopen.o: ../h/types.h
archindep/sysopen.o: archindep/sysopen.c
archindep/test.o: ../h/a.out.h
archindep/test.o: ../h/arp.h
archindep/test.o: ../h/conf.h
archindep/test.o: ../h/daemon.h
archindep/test.o: ../h/dgram.h
archindep/test.o: ../h/domain.h
archindep/test.o: ../h/ether.h
archindep/test.o: ../h/file.h
archindep/test.o: ../h/fserver.h
archindep/test.o: ../h/icmp.h
archindep/test.o: ../h/ip.h
archindep/test.o: ../h/ipreass.h
archindep/test.o: ../h/kernel.h
archindep/test.o: ../h/kernel.sparc.h
archindep/test.o: ../h/mem.h
archindep/test.o: ../h/net.h
archindep/test.o: ../h/netconf.h
archindep/test.o: ../h/netif.h
archindep/test.o: ../h/network.h
archindep/test.o: ../h/proc.h
archindep/test.o: ../h/rfile.h
archindep/test.o: ../h/rip.h
archindep/test.o: ../h/route.h
archindep/test.o: ../h/select.h
archindep/test.o: ../h/snmpvars.h
archindep/test.o: ../h/tp.h
archindep/test.o: ../h/types.h
archindep/test.o: ../h/udp.h
archindep/test.o: ../h/vmem.h
archindep/test.o: ../h/vmem.sparc.h
archindep/test.o: ../h/xpppgio.h
archindep/test.o: archindep/test.c
archindep/unmount.o: ../h/conf.h
archindep/unmount.o: ../h/kernel.h
archindep/unmount.o: ../h/kernel.sparc.h
archindep/unmount.o: ../h/name.h
archindep/unmount.o: ../h/types.h
archindep/unmount.o: archindep/unmount.c
archindep/unsleep.o: ../h/a.out.h
archindep/unsleep.o: ../h/conf.h
archindep/unsleep.o: ../h/kernel.h
archindep/unsleep.o: ../h/kernel.sparc.h
archindep/unsleep.o: ../h/proc.h
archindep/unsleep.o: ../h/q.h
archindep/unsleep.o: ../h/select.h
archindep/unsleep.o: ../h/sleep.h
archindep/unsleep.o: ../h/types.h
archindep/unsleep.o: archindep/unsleep.c
archindep/userret.o: ../h/conf.h
archindep/userret.o: ../h/kernel.h
archindep/userret.o: ../h/kernel.sparc.h
archindep/userret.o: ../h/types.h
archindep/userret.o: archindep/userret.c
archindep/wait.o: ../h/a.out.h
archindep/wait.o: ../h/conf.h
archindep/wait.o: ../h/kernel.h
archindep/wait.o: ../h/kernel.sparc.h
archindep/wait.o: ../h/proc.h
archindep/wait.o: ../h/q.h
archindep/wait.o: ../h/select.h
archindep/wait.o: ../h/sem.h
archindep/wait.o: ../h/types.h
archindep/wait.o: archindep/wait.c
archindep/wakeup.o: ../h/a.out.h
archindep/wakeup.o: ../h/conf.h
archindep/wakeup.o: ../h/kernel.h
archindep/wakeup.o: ../h/kernel.sparc.h
archindep/wakeup.o: ../h/proc.h
archindep/wakeup.o: ../h/q.h
archindep/wakeup.o: ../h/select.h
archindep/wakeup.o: ../h/sleep.h
archindep/wakeup.o: ../h/types.h
archindep/wakeup.o: archindep/wakeup.c
archindep/write.o: ../h/conf.h
archindep/write.o: ../h/io.h
archindep/write.o: ../h/kernel.h
archindep/write.o: ../h/kernel.sparc.h
archindep/write.o: ../h/types.h
archindep/write.o: archindep/write.c
archindep/xdone.o: ../h/kernel.h
archindep/xdone.o: ../h/kernel.sparc.h
archindep/xdone.o: ../h/types.h
archindep/xdone.o: archindep/xdone.c
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
