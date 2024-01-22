#
#  Makefile for VM Xinu device drivers
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

OLIB	=	xdevs.o

AFILE	=	../libx.a
TSTAMP	=	.last_make_all_run

DIR1	=	ethernet
DIR2	=	tty
DIR3	=	clock

CLOCKOFIL = 	clock/clkinit.o		\
		clock/clkint.o

ETHERNETOFIL = 	ethernet/ethinit.o	ethernet/ethinter.o	\
		ethernet/ethread.o	ethernet/ethstrt.o	\
		ethernet/ethwrite.o	ethernet/ethwstrt.o	\
		ethernet/ethcntl.o	\
		ethernet/ethdemux.o

TTYOFIL	= 	tty/ttycntl.o		\
		tty/ttygetc.o		\
		tty/ttyiin.o		\
		tty/ttyinit.o		\
		tty/ttyoin.o		\
		tty/ttyputc.o		\
		tty/ttyread.o		\
		tty/ttyutils.o		\
		tty/ttywrite.o		\
		tty/ttyint.o

NULLOFIL =	null/null.o

PIPEOFIL =	pipe/pipeclose.o	pipe/pipegetc.o		\
		pipe/pipeopen.o		pipe/piperead.o		\
		pipe/pipecntl.o		pipe/pipeinit.o	\
		pipe/pipeputc.o		pipe/pipewrite.o

ALLOFIL	=	${CLOCKOFIL} ${TTYOFIL} ${ETHERNETOFIL} ${NULLOFIL} ${PIPEOFIL}

.c.o:
	${CC} ${CFLAGS} -o $@ $<
.s.o:
	/lib/cpp ${ASCPPFLAGS} $< > .tmp.s
	${CC} -c ${SFLAGS} -o $@ .tmp.s

all:	${TSTAMP}

${TSTAMP}: ${ALLOFIL}
#	$(AR) cr ${AFILE} ${ALLOFIL}
	ld -r ${ALLOFIL} -o xdevs.o
	touch ${TSTAMP}

top:
	cd .. ; make

ethernet: ${ETHERNETOFIL}
	$(AR) cr ${AFILE} ${ETHERNETOFIL}

tty:	${TTYOFIL}
	$(AR) cr ${AFILE} ${TTYOFIL}

clock:	${CLOCKOFIL}
	$(AR) cr ${AFILE} ${CLOCKOFIL}

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
	${MAKETD} -d -o${DIR3} ${CDEFS} -I${HDIR} ${DIR3}/*.[cs] >> makedep
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
ethernet/ethcntl.o: ../h/arp.h
ethernet/ethcntl.o: ../h/conf.h
ethernet/ethcntl.o: ../h/daemon.h
ethernet/ethcntl.o: ../h/dgram.h
ethernet/ethcntl.o: ../h/domain.h
ethernet/ethcntl.o: ../h/ether.h
ethernet/ethcntl.o: ../h/file.h
ethernet/ethcntl.o: ../h/fserver.h
ethernet/ethcntl.o: ../h/icmp.h
ethernet/ethcntl.o: ../h/ip.h
ethernet/ethcntl.o: ../h/ipreass.h
ethernet/ethcntl.o: ../h/kernel.h
ethernet/ethcntl.o: ../h/kernel.sparc.h
ethernet/ethcntl.o: ../h/lereg.h
ethernet/ethcntl.o: ../h/net.h
ethernet/ethcntl.o: ../h/netconf.h
ethernet/ethcntl.o: ../h/netif.h
ethernet/ethcntl.o: ../h/network.h
ethernet/ethcntl.o: ../h/rfile.h
ethernet/ethcntl.o: ../h/rip.h
ethernet/ethcntl.o: ../h/route.h
ethernet/ethcntl.o: ../h/snmpvars.h
ethernet/ethcntl.o: ../h/types.h
ethernet/ethcntl.o: ../h/udp.h
ethernet/ethcntl.o: ethernet/ethcntl.c
ethernet/ethdemux.o: ../h/a.out.h
ethernet/ethdemux.o: ../h/arp.h
ethernet/ethdemux.o: ../h/conf.h
ethernet/ethdemux.o: ../h/daemon.h
ethernet/ethdemux.o: ../h/dgram.h
ethernet/ethdemux.o: ../h/domain.h
ethernet/ethdemux.o: ../h/ether.h
ethernet/ethdemux.o: ../h/file.h
ethernet/ethdemux.o: ../h/fserver.h
ethernet/ethdemux.o: ../h/icmp.h
ethernet/ethdemux.o: ../h/ip.h
ethernet/ethdemux.o: ../h/ipreass.h
ethernet/ethdemux.o: ../h/kernel.h
ethernet/ethdemux.o: ../h/kernel.sparc.h
ethernet/ethdemux.o: ../h/lereg.h
ethernet/ethdemux.o: ../h/net.h
ethernet/ethdemux.o: ../h/netconf.h
ethernet/ethdemux.o: ../h/netif.h
ethernet/ethdemux.o: ../h/network.h
ethernet/ethdemux.o: ../h/proc.h
ethernet/ethdemux.o: ../h/rfile.h
ethernet/ethdemux.o: ../h/rip.h
ethernet/ethdemux.o: ../h/route.h
ethernet/ethdemux.o: ../h/select.h
ethernet/ethdemux.o: ../h/snmpvars.h
ethernet/ethdemux.o: ../h/types.h
ethernet/ethdemux.o: ../h/udp.h
ethernet/ethdemux.o: ../h/vmem.h
ethernet/ethdemux.o: ../h/vmem.sparc.h
ethernet/ethdemux.o: ethernet/ethdemux.c
ethernet/ethinit.o: ../h/a.out.h
ethernet/ethinit.o: ../h/arp.h
ethernet/ethinit.o: ../h/conf.h
ethernet/ethinit.o: ../h/daemon.h
ethernet/ethinit.o: ../h/dgram.h
ethernet/ethinit.o: ../h/domain.h
ethernet/ethinit.o: ../h/ether.h
ethernet/ethinit.o: ../h/file.h
ethernet/ethinit.o: ../h/fserver.h
ethernet/ethinit.o: ../h/icmp.h
ethernet/ethinit.o: ../h/idprom.h
ethernet/ethinit.o: ../h/ip.h
ethernet/ethinit.o: ../h/ipreass.h
ethernet/ethinit.o: ../h/kernel.h
ethernet/ethinit.o: ../h/kernel.sparc.h
ethernet/ethinit.o: ../h/lereg.h
ethernet/ethinit.o: ../h/net.h
ethernet/ethinit.o: ../h/netconf.h
ethernet/ethinit.o: ../h/netif.h
ethernet/ethinit.o: ../h/network.h
ethernet/ethinit.o: ../h/proc.h
ethernet/ethinit.o: ../h/q.h
ethernet/ethinit.o: ../h/rfile.h
ethernet/ethinit.o: ../h/rip.h
ethernet/ethinit.o: ../h/route.h
ethernet/ethinit.o: ../h/select.h
ethernet/ethinit.o: ../h/snmpvars.h
ethernet/ethinit.o: ../h/types.h
ethernet/ethinit.o: ../h/udp.h
ethernet/ethinit.o: ../h/vmem.h
ethernet/ethinit.o: ../h/vmem.sparc.h
ethernet/ethinit.o: ethernet/ethinit.c
ethernet/ethint.o: ../h/arp.h
ethernet/ethint.o: ../h/conf.h
ethernet/ethint.o: ../h/daemon.h
ethernet/ethint.o: ../h/dgram.h
ethernet/ethint.o: ../h/domain.h
ethernet/ethint.o: ../h/ether.h
ethernet/ethint.o: ../h/file.h
ethernet/ethint.o: ../h/fserver.h
ethernet/ethint.o: ../h/icmp.h
ethernet/ethint.o: ../h/ip.h
ethernet/ethint.o: ../h/ipreass.h
ethernet/ethint.o: ../h/kernel.h
ethernet/ethint.o: ../h/kernel.sparc.h
ethernet/ethint.o: ../h/net.h
ethernet/ethint.o: ../h/netconf.h
ethernet/ethint.o: ../h/netif.h
ethernet/ethint.o: ../h/network.h
ethernet/ethint.o: ../h/rfile.h
ethernet/ethint.o: ../h/rip.h
ethernet/ethint.o: ../h/route.h
ethernet/ethint.o: ../h/snmpvars.h
ethernet/ethint.o: ../h/types.h
ethernet/ethint.o: ../h/udp.h
ethernet/ethint.o: ethernet/ethint.c
ethernet/ethinter.o: ../h/a.out.h
ethernet/ethinter.o: ../h/arp.h
ethernet/ethinter.o: ../h/conf.h
ethernet/ethinter.o: ../h/daemon.h
ethernet/ethinter.o: ../h/dgram.h
ethernet/ethinter.o: ../h/domain.h
ethernet/ethinter.o: ../h/ether.h
ethernet/ethinter.o: ../h/file.h
ethernet/ethinter.o: ../h/fserver.h
ethernet/ethinter.o: ../h/icmp.h
ethernet/ethinter.o: ../h/ip.h
ethernet/ethinter.o: ../h/ipreass.h
ethernet/ethinter.o: ../h/kernel.h
ethernet/ethinter.o: ../h/kernel.sparc.h
ethernet/ethinter.o: ../h/lereg.h
ethernet/ethinter.o: ../h/net.h
ethernet/ethinter.o: ../h/netconf.h
ethernet/ethinter.o: ../h/netif.h
ethernet/ethinter.o: ../h/network.h
ethernet/ethinter.o: ../h/proc.h
ethernet/ethinter.o: ../h/q.h
ethernet/ethinter.o: ../h/rfile.h
ethernet/ethinter.o: ../h/rip.h
ethernet/ethinter.o: ../h/route.h
ethernet/ethinter.o: ../h/select.h
ethernet/ethinter.o: ../h/snmpvars.h
ethernet/ethinter.o: ../h/types.h
ethernet/ethinter.o: ../h/udp.h
ethernet/ethinter.o: ../h/vmem.h
ethernet/ethinter.o: ../h/vmem.sparc.h
ethernet/ethinter.o: ethernet/ethinter.c
ethernet/ethread.o: ../h/a.out.h
ethernet/ethread.o: ../h/arp.h
ethernet/ethread.o: ../h/conf.h
ethernet/ethread.o: ../h/daemon.h
ethernet/ethread.o: ../h/dgram.h
ethernet/ethread.o: ../h/domain.h
ethernet/ethread.o: ../h/ether.h
ethernet/ethread.o: ../h/file.h
ethernet/ethread.o: ../h/fserver.h
ethernet/ethread.o: ../h/icmp.h
ethernet/ethread.o: ../h/ip.h
ethernet/ethread.o: ../h/ipreass.h
ethernet/ethread.o: ../h/kernel.h
ethernet/ethread.o: ../h/kernel.sparc.h
ethernet/ethread.o: ../h/lereg.h
ethernet/ethread.o: ../h/net.h
ethernet/ethread.o: ../h/netconf.h
ethernet/ethread.o: ../h/netif.h
ethernet/ethread.o: ../h/network.h
ethernet/ethread.o: ../h/proc.h
ethernet/ethread.o: ../h/q.h
ethernet/ethread.o: ../h/rfile.h
ethernet/ethread.o: ../h/rip.h
ethernet/ethread.o: ../h/route.h
ethernet/ethread.o: ../h/select.h
ethernet/ethread.o: ../h/snmpvars.h
ethernet/ethread.o: ../h/types.h
ethernet/ethread.o: ../h/udp.h
ethernet/ethread.o: ethernet/ethread.c
ethernet/ethstrt.o: ../h/arp.h
ethernet/ethstrt.o: ../h/conf.h
ethernet/ethstrt.o: ../h/daemon.h
ethernet/ethstrt.o: ../h/dgram.h
ethernet/ethstrt.o: ../h/domain.h
ethernet/ethstrt.o: ../h/ether.h
ethernet/ethstrt.o: ../h/file.h
ethernet/ethstrt.o: ../h/fserver.h
ethernet/ethstrt.o: ../h/icmp.h
ethernet/ethstrt.o: ../h/idprom.h
ethernet/ethstrt.o: ../h/ip.h
ethernet/ethstrt.o: ../h/ipreass.h
ethernet/ethstrt.o: ../h/kernel.h
ethernet/ethstrt.o: ../h/kernel.sparc.h
ethernet/ethstrt.o: ../h/lereg.h
ethernet/ethstrt.o: ../h/net.h
ethernet/ethstrt.o: ../h/netconf.h
ethernet/ethstrt.o: ../h/netif.h
ethernet/ethstrt.o: ../h/network.h
ethernet/ethstrt.o: ../h/rfile.h
ethernet/ethstrt.o: ../h/rip.h
ethernet/ethstrt.o: ../h/route.h
ethernet/ethstrt.o: ../h/snmpvars.h
ethernet/ethstrt.o: ../h/types.h
ethernet/ethstrt.o: ../h/udp.h
ethernet/ethstrt.o: ../h/vmem.h
ethernet/ethstrt.o: ../h/vmem.sparc.h
ethernet/ethstrt.o: ethernet/ethstrt.c
ethernet/ethwrite.o: ../h/a.out.h
ethernet/ethwrite.o: ../h/arp.h
ethernet/ethwrite.o: ../h/conf.h
ethernet/ethwrite.o: ../h/daemon.h
ethernet/ethwrite.o: ../h/dgram.h
ethernet/ethwrite.o: ../h/domain.h
ethernet/ethwrite.o: ../h/ether.h
ethernet/ethwrite.o: ../h/file.h
ethernet/ethwrite.o: ../h/fserver.h
ethernet/ethwrite.o: ../h/icmp.h
ethernet/ethwrite.o: ../h/ip.h
ethernet/ethwrite.o: ../h/ipreass.h
ethernet/ethwrite.o: ../h/kernel.h
ethernet/ethwrite.o: ../h/kernel.sparc.h
ethernet/ethwrite.o: ../h/net.h
ethernet/ethwrite.o: ../h/netconf.h
ethernet/ethwrite.o: ../h/netif.h
ethernet/ethwrite.o: ../h/network.h
ethernet/ethwrite.o: ../h/proc.h
ethernet/ethwrite.o: ../h/rfile.h
ethernet/ethwrite.o: ../h/rip.h
ethernet/ethwrite.o: ../h/route.h
ethernet/ethwrite.o: ../h/select.h
ethernet/ethwrite.o: ../h/snmpvars.h
ethernet/ethwrite.o: ../h/types.h
ethernet/ethwrite.o: ../h/udp.h
ethernet/ethwrite.o: ethernet/ethwrite.c
ethernet/ethwstrt.o: ../h/a.out.h
ethernet/ethwstrt.o: ../h/arp.h
ethernet/ethwstrt.o: ../h/conf.h
ethernet/ethwstrt.o: ../h/daemon.h
ethernet/ethwstrt.o: ../h/dgram.h
ethernet/ethwstrt.o: ../h/domain.h
ethernet/ethwstrt.o: ../h/ether.h
ethernet/ethwstrt.o: ../h/file.h
ethernet/ethwstrt.o: ../h/fserver.h
ethernet/ethwstrt.o: ../h/icmp.h
ethernet/ethwstrt.o: ../h/ip.h
ethernet/ethwstrt.o: ../h/ipreass.h
ethernet/ethwstrt.o: ../h/kernel.h
ethernet/ethwstrt.o: ../h/kernel.sparc.h
ethernet/ethwstrt.o: ../h/lereg.h
ethernet/ethwstrt.o: ../h/net.h
ethernet/ethwstrt.o: ../h/netconf.h
ethernet/ethwstrt.o: ../h/netif.h
ethernet/ethwstrt.o: ../h/network.h
ethernet/ethwstrt.o: ../h/proc.h
ethernet/ethwstrt.o: ../h/rfile.h
ethernet/ethwstrt.o: ../h/rip.h
ethernet/ethwstrt.o: ../h/route.h
ethernet/ethwstrt.o: ../h/select.h
ethernet/ethwstrt.o: ../h/snmpvars.h
ethernet/ethwstrt.o: ../h/types.h
ethernet/ethwstrt.o: ../h/udp.h
ethernet/ethwstrt.o: ../h/vmem.h
ethernet/ethwstrt.o: ../h/vmem.sparc.h
ethernet/ethwstrt.o: ethernet/ethwstrt.c
tty/ttycntl.o: ../h/conf.h
tty/ttycntl.o: ../h/io.h
tty/ttycntl.o: ../h/kernel.h
tty/ttycntl.o: ../h/kernel.sparc.h
tty/ttycntl.o: ../h/slu.h
tty/ttycntl.o: ../h/tty.h
tty/ttycntl.o: ../h/types.h
tty/ttycntl.o: tty/ttycntl.c
tty/ttygetc.o: ../h/conf.h
tty/ttygetc.o: ../h/io.h
tty/ttygetc.o: ../h/kernel.h
tty/ttygetc.o: ../h/kernel.sparc.h
tty/ttygetc.o: ../h/tty.h
tty/ttygetc.o: ../h/types.h
tty/ttygetc.o: tty/ttygetc.c
tty/ttyiin.o: ../h/conf.h
tty/ttyiin.o: ../h/io.h
tty/ttyiin.o: ../h/kernel.h
tty/ttyiin.o: ../h/kernel.sparc.h
tty/ttyiin.o: ../h/slu.h
tty/ttyiin.o: ../h/tty.h
tty/ttyiin.o: ../h/types.h
tty/ttyiin.o: ../h/zsreg.h
tty/ttyiin.o: tty/ttyiin.c
tty/ttyinit.o: ../h/a.out.h
tty/ttyinit.o: ../h/conf.h
tty/ttyinit.o: ../h/io.h
tty/ttyinit.o: ../h/kernel.h
tty/ttyinit.o: ../h/kernel.sparc.h
tty/ttyinit.o: ../h/proc.h
tty/ttyinit.o: ../h/select.h
tty/ttyinit.o: ../h/slu.h
tty/ttyinit.o: ../h/tty.h
tty/ttyinit.o: ../h/types.h
tty/ttyinit.o: ../h/zsreg.h
tty/ttyinit.o: tty/ttyinit.c
tty/ttyint.o: ../h/conf.h
tty/ttyint.o: ../h/io.h
tty/ttyint.o: ../h/kernel.h
tty/ttyint.o: ../h/kernel.sparc.h
tty/ttyint.o: ../h/slu.h
tty/ttyint.o: ../h/tty.h
tty/ttyint.o: ../h/types.h
tty/ttyint.o: ../h/zsreg.h
tty/ttyint.o: tty/ttyint.c
tty/ttyoin.o: ../h/conf.h
tty/ttyoin.o: ../h/io.h
tty/ttyoin.o: ../h/kernel.h
tty/ttyoin.o: ../h/kernel.sparc.h
tty/ttyoin.o: ../h/slu.h
tty/ttyoin.o: ../h/tty.h
tty/ttyoin.o: ../h/types.h
tty/ttyoin.o: ../h/zsreg.h
tty/ttyoin.o: tty/ttyoin.c
tty/ttyputc.o: ../h/conf.h
tty/ttyputc.o: ../h/io.h
tty/ttyputc.o: ../h/kernel.h
tty/ttyputc.o: ../h/kernel.sparc.h
tty/ttyputc.o: ../h/slu.h
tty/ttyputc.o: ../h/tty.h
tty/ttyputc.o: ../h/types.h
tty/ttyputc.o: ../h/zsreg.h
tty/ttyputc.o: tty/ttyputc.c
tty/ttyread.o: ../h/conf.h
tty/ttyread.o: ../h/io.h
tty/ttyread.o: ../h/kernel.h
tty/ttyread.o: ../h/kernel.sparc.h
tty/ttyread.o: ../h/tty.h
tty/ttyread.o: ../h/types.h
tty/ttyread.o: tty/ttyread.c
tty/ttyutils.o: ../h/conf.h
tty/ttyutils.o: ../h/io.h
tty/ttyutils.o: ../h/kernel.h
tty/ttyutils.o: ../h/kernel.sparc.h
tty/ttyutils.o: ../h/slu.h
tty/ttyutils.o: ../h/tty.h
tty/ttyutils.o: ../h/types.h
tty/ttyutils.o: ../h/zsreg.h
tty/ttyutils.o: tty/ttyutils.c
tty/ttywrite.o: ../h/conf.h
tty/ttywrite.o: ../h/io.h
tty/ttywrite.o: ../h/kernel.h
tty/ttywrite.o: ../h/kernel.sparc.h
tty/ttywrite.o: ../h/slu.h
tty/ttywrite.o: ../h/tty.h
tty/ttywrite.o: ../h/types.h
tty/ttywrite.o: tty/ttywrite.c
clock/clkinit.o: ../h/clock.h
clock/clkinit.o: ../h/conf.h
clock/clkinit.o: ../h/interreg.h
clock/clkinit.o: ../h/kernel.h
clock/clkinit.o: ../h/kernel.sparc.h
clock/clkinit.o: ../h/mem.h
clock/clkinit.o: ../h/sleep.h
clock/clkinit.o: ../h/types.h
clock/clkinit.o: ../h/vmem.h
clock/clkinit.o: ../h/vmem.sparc.h
clock/clkinit.o: clock/clkinit.c
clock/clkint.o: ../h/clock.h
clock/clkint.o: ../h/conf.h
clock/clkint.o: ../h/interreg.h
clock/clkint.o: ../h/kernel.h
clock/clkint.o: ../h/kernel.sparc.h
clock/clkint.o: ../h/mem.h
clock/clkint.o: ../h/sleep.h
clock/clkint.o: ../h/types.h
clock/clkint.o: ../h/vmem.h
clock/clkint.o: ../h/vmem.sparc.h
clock/clkint.o: clock/clkint.c
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
