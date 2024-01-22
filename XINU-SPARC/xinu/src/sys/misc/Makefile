#
#  Makefile for VM Xinu misc routines
#

# Directory where we are building Xinu
DIR	=	../../..

# Machine specific things we must define
INSTR_TYPE	=	RISC
ARCH		=	SPARC
MODEL		=	SPARC_SLC
CODE_NAME	=	sun4c


CDEFS	=	-D$(INSTR_TYPE) -D$(ARCH) -D$(MODEL) -D$(CODE_NAME) -DKERNEL
#CFLAGS	=	-g -c -I../h ${CDEFS}
CFLAGS	=	-O -c -I../h ${CDEFS}
SFLAGS	=	-I../h
ASCPPFLAGS =	-I../h -DASM ${CDEFS}
AR	=	/bin/ar
CC	=	/bin/cc
LD	=	/bin/ld
LIB	=	${DIR}/lib
HDIR	=	../h
MAKETD	= 	../../tools/maketd

OLIB	=	xmisc.o

AFILE	=	../libx.a
TSTAMP	=	.last_make_all_run

DIR1	=	debug
DIR2	=	etc

DEBUGOFIL = 	debug/debug.o debug/stktrace.o debug/dprintf.o \
		debug/kprintip.o

ETCOFIL	= 	etc/prfunctions.o etc/printheap.o etc/printpgtbl.o	\
		etc/printq.o etc/prsem.o

ALLOFIL	=	${DEBUGOFIL} ${ETCOFIL}

.c.o:
	${CC} ${CFLAGS} -o $@ $<
.s.o:
	/lib/cpp ${ASCPPFLAGS} $< > .tmp.s
	${CC} -c ${SFLAGS} -o $@ .tmp.s


all:	${TSTAMP}

${TSTAMP}: ${ALLOFIL}
#	$(AR) cr ${AFILE} ${ALLOFIL}
	ld -r ${ALLOFIL} -o xmisc.o
	touch ${TSTAMP}

top:
	cd .. ; make

debug:	${DEBUGOFIL}
	$(AR) cr ${AFILE} ${DEBUGOFIL}

etc:	${ETCOFIL}
	$(AR) cr ${AFILE} ${ETCOFIL}

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
debug/debug.o: ../h/conf.h
debug/debug.o: ../h/kernel.h
debug/debug.o: ../h/kernel.sparc.h
debug/debug.o: ../h/types.h
debug/debug.o: debug/debug.c
debug/dprintf.o: ../h/conf.h
debug/dprintf.o: ../h/kernel.h
debug/dprintf.o: ../h/kernel.sparc.h
debug/dprintf.o: ../h/types.h
debug/dprintf.o: debug/dprintf.c
debug/kprintip.o: ../h/arp.h
debug/kprintip.o: ../h/conf.h
debug/kprintip.o: ../h/daemon.h
debug/kprintip.o: ../h/dgram.h
debug/kprintip.o: ../h/domain.h
debug/kprintip.o: ../h/ether.h
debug/kprintip.o: ../h/file.h
debug/kprintip.o: ../h/fserver.h
debug/kprintip.o: ../h/icmp.h
debug/kprintip.o: ../h/ip.h
debug/kprintip.o: ../h/ipreass.h
debug/kprintip.o: ../h/kernel.h
debug/kprintip.o: ../h/kernel.sparc.h
debug/kprintip.o: ../h/net.h
debug/kprintip.o: ../h/netconf.h
debug/kprintip.o: ../h/netif.h
debug/kprintip.o: ../h/network.h
debug/kprintip.o: ../h/rfile.h
debug/kprintip.o: ../h/rip.h
debug/kprintip.o: ../h/route.h
debug/kprintip.o: ../h/snmpvars.h
debug/kprintip.o: ../h/types.h
debug/kprintip.o: ../h/udp.h
debug/kprintip.o: debug/kprintip.c
debug/stktrace.o: ../h/a.out.h
debug/stktrace.o: ../h/conf.h
debug/stktrace.o: ../h/kernel.h
debug/stktrace.o: ../h/kernel.sparc.h
debug/stktrace.o: ../h/proc.h
debug/stktrace.o: ../h/select.h
debug/stktrace.o: ../h/types.h
debug/stktrace.o: ../h/vmem.h
debug/stktrace.o: ../h/vmem.sparc.h
debug/stktrace.o: debug/stktrace.c
etc/prfunctions.o: ../h/a.out.h
etc/prfunctions.o: ../h/arp.h
etc/prfunctions.o: ../h/conf.h
etc/prfunctions.o: ../h/daemon.h
etc/prfunctions.o: ../h/dgram.h
etc/prfunctions.o: ../h/domain.h
etc/prfunctions.o: ../h/ether.h
etc/prfunctions.o: ../h/file.h
etc/prfunctions.o: ../h/fserver.h
etc/prfunctions.o: ../h/icmp.h
etc/prfunctions.o: ../h/io.h
etc/prfunctions.o: ../h/ip.h
etc/prfunctions.o: ../h/ipreass.h
etc/prfunctions.o: ../h/kernel.h
etc/prfunctions.o: ../h/kernel.sparc.h
etc/prfunctions.o: ../h/mem.h
etc/prfunctions.o: ../h/net.h
etc/prfunctions.o: ../h/netconf.h
etc/prfunctions.o: ../h/netif.h
etc/prfunctions.o: ../h/network.h
etc/prfunctions.o: ../h/pgio.h
etc/prfunctions.o: ../h/proc.h
etc/prfunctions.o: ../h/q.h
etc/prfunctions.o: ../h/rfile.h
etc/prfunctions.o: ../h/rip.h
etc/prfunctions.o: ../h/route.h
etc/prfunctions.o: ../h/select.h
etc/prfunctions.o: ../h/sleep.h
etc/prfunctions.o: ../h/snmpvars.h
etc/prfunctions.o: ../h/tty.h
etc/prfunctions.o: ../h/types.h
etc/prfunctions.o: ../h/udp.h
etc/prfunctions.o: ../h/vmem.h
etc/prfunctions.o: ../h/vmem.sparc.h
etc/prfunctions.o: etc/prfunctions.c
etc/printheap.o: ../h/a.out.h
etc/printheap.o: ../h/conf.h
etc/printheap.o: ../h/kernel.h
etc/printheap.o: ../h/kernel.sparc.h
etc/printheap.o: ../h/mem.h
etc/printheap.o: ../h/proc.h
etc/printheap.o: ../h/select.h
etc/printheap.o: ../h/types.h
etc/printheap.o: ../h/vmem.h
etc/printheap.o: ../h/vmem.sparc.h
etc/printheap.o: etc/printheap.c
etc/printpgtbl.o: ../h/a.out.h
etc/printpgtbl.o: ../h/conf.h
etc/printpgtbl.o: ../h/frame.h
etc/printpgtbl.o: ../h/kernel.h
etc/printpgtbl.o: ../h/kernel.sparc.h
etc/printpgtbl.o: ../h/mem.h
etc/printpgtbl.o: ../h/proc.h
etc/printpgtbl.o: ../h/select.h
etc/printpgtbl.o: ../h/types.h
etc/printpgtbl.o: ../h/vmem.h
etc/printpgtbl.o: ../h/vmem.sparc.h
etc/printpgtbl.o: etc/printpgtbl.c
etc/printq.o: ../h/q.h
etc/printq.o: etc/printq.c
etc/prsem.o: ../h/a.out.h
etc/prsem.o: ../h/conf.h
etc/prsem.o: ../h/kernel.h
etc/prsem.o: ../h/kernel.sparc.h
etc/prsem.o: ../h/proc.h
etc/prsem.o: ../h/q.h
etc/prsem.o: ../h/select.h
etc/prsem.o: ../h/sem.h
etc/prsem.o: ../h/types.h
etc/prsem.o: etc/prsem.c
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
