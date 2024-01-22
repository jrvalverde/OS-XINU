#
#  Makefile for VM Xinu virtual memory routines
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

OLIB	=	xvm.o

TSTAMP	=	.last_make_all_run

DIR1	=	archindep
DIR2	=	archdep
DIR3	=	fm
DIR4	=	paging
DIR5	=	syscall

DEPOFIL =	archdep/getppte.o 		\
		archdep/pmeg.o			\
		archdep/lmmu.o			\
		archdep/hmmu.o			\
		archdep/vac.o			\
		archdep/vacreg.o		\
		archdep/pmegdump.o		\
		archdep/vminit.o		

INDEPOFIL =	archindep/freeheap.o		\
		archindep/getheap.o		\
		archindep/pfault.o		\
		archindep/pgstats.o		\
		archindep/physpgcopy.o		\
		archindep/meminfo.o		\
		archindep/vmrpalloc.o		\
		archindep/vbcopy.o

FMOFIL	=	fm/fmdump.o			\
		fm/fmtblinit.o			\
		fm/fmutil.o			\
		fm/getframe.o			\
		fm/mapframe.o			\
		fm/pagereplace.o

PAGEOFIL =	paging/pgio.o			\
		paging/tprotocol.o		\
		paging/xpppgio.o		\
		paging/nfspgio.o

SCALLOFIL =	syscall/syscall.o

ALLOFIL	=	${IFACEOFIL} ${INDEPOFIL} ${DEPOFIL} ${FMOFIL} \
		${PAGEOFIL} ${SCALLOFIL} 

.c.o:
	${CC} ${CFLAGS} -o $@ $<
.s.o:
	/lib/cpp ${ASCPPFLAGS} $< > .tmp.s
	${CC} -c ${SFLAGS} -o $@ .tmp.s

all:	${TSTAMP}

${TSTAMP}: ${ALLOFIL}
	ld -r ${ALLOFIL} -o xvm.o
	touch ${TSTAMP}

top:
	cd .. ; make

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
	${MAKETD} -d -o${DIR4} ${CDEFS} -I${HDIR} ${DIR4}/*.[cs] >> makedep
	${MAKETD} -d -o${DIR5} ${CDEFS} -I${HDIR} ${DIR5}/*.[cs] >> makedep
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
archindep/freeheap.o: ../h/a.out.h
archindep/freeheap.o: ../h/conf.h
archindep/freeheap.o: ../h/kernel.h
archindep/freeheap.o: ../h/kernel.sparc.h
archindep/freeheap.o: ../h/mem.h
archindep/freeheap.o: ../h/proc.h
archindep/freeheap.o: ../h/select.h
archindep/freeheap.o: ../h/types.h
archindep/freeheap.o: ../h/vmem.h
archindep/freeheap.o: ../h/vmem.sparc.h
archindep/freeheap.o: archindep/freeheap.c
archindep/getheap.o: ../h/a.out.h
archindep/getheap.o: ../h/conf.h
archindep/getheap.o: ../h/kernel.h
archindep/getheap.o: ../h/kernel.sparc.h
archindep/getheap.o: ../h/mem.h
archindep/getheap.o: ../h/proc.h
archindep/getheap.o: ../h/select.h
archindep/getheap.o: ../h/types.h
archindep/getheap.o: ../h/vmem.h
archindep/getheap.o: ../h/vmem.sparc.h
archindep/getheap.o: archindep/getheap.c
archindep/meminfo.o: ../h/a.out.h
archindep/meminfo.o: ../h/conf.h
archindep/meminfo.o: ../h/frame.h
archindep/meminfo.o: ../h/kernel.h
archindep/meminfo.o: ../h/kernel.sparc.h
archindep/meminfo.o: ../h/mem.h
archindep/meminfo.o: ../h/proc.h
archindep/meminfo.o: ../h/select.h
archindep/meminfo.o: ../h/types.h
archindep/meminfo.o: ../h/vmem.h
archindep/meminfo.o: ../h/vmem.sparc.h
archindep/meminfo.o: archindep/meminfo.c
archindep/pfault.o: ../h/a.out.h
archindep/pfault.o: ../h/conf.h
archindep/pfault.o: ../h/frame.h
archindep/pfault.o: ../h/kernel.h
archindep/pfault.o: ../h/kernel.sparc.h
archindep/pfault.o: ../h/proc.h
archindep/pfault.o: ../h/select.h
archindep/pfault.o: ../h/sem.h
archindep/pfault.o: ../h/types.h
archindep/pfault.o: ../h/vmem.h
archindep/pfault.o: ../h/vmem.sparc.h
archindep/pfault.o: archindep/pfault.c
archindep/pgstats.o: ../h/a.out.h
archindep/pgstats.o: ../h/conf.h
archindep/pgstats.o: ../h/kernel.h
archindep/pgstats.o: ../h/kernel.sparc.h
archindep/pgstats.o: ../h/mem.h
archindep/pgstats.o: ../h/pgio.h
archindep/pgstats.o: ../h/proc.h
archindep/pgstats.o: ../h/select.h
archindep/pgstats.o: ../h/types.h
archindep/pgstats.o: ../h/vmem.h
archindep/pgstats.o: ../h/vmem.sparc.h
archindep/pgstats.o: archindep/pgstats.c
archindep/physpgcopy.o: ../h/a.out.h
archindep/physpgcopy.o: ../h/conf.h
archindep/physpgcopy.o: ../h/kernel.h
archindep/physpgcopy.o: ../h/kernel.sparc.h
archindep/physpgcopy.o: ../h/mem.h
archindep/physpgcopy.o: ../h/proc.h
archindep/physpgcopy.o: ../h/q.h
archindep/physpgcopy.o: ../h/select.h
archindep/physpgcopy.o: ../h/types.h
archindep/physpgcopy.o: ../h/vmem.h
archindep/physpgcopy.o: ../h/vmem.sparc.h
archindep/physpgcopy.o: archindep/physpgcopy.c
archindep/vbcopy.o: ../h/a.out.h
archindep/vbcopy.o: ../h/conf.h
archindep/vbcopy.o: ../h/kernel.h
archindep/vbcopy.o: ../h/kernel.sparc.h
archindep/vbcopy.o: ../h/mem.h
archindep/vbcopy.o: ../h/proc.h
archindep/vbcopy.o: ../h/q.h
archindep/vbcopy.o: ../h/select.h
archindep/vbcopy.o: ../h/types.h
archindep/vbcopy.o: ../h/vmem.h
archindep/vbcopy.o: ../h/vmem.sparc.h
archindep/vbcopy.o: archindep/vbcopy.c
archindep/vmrpalloc.o: ../h/conf.h
archindep/vmrpalloc.o: ../h/kernel.h
archindep/vmrpalloc.o: ../h/kernel.sparc.h
archindep/vmrpalloc.o: ../h/mark.h
archindep/vmrpalloc.o: ../h/types.h
archindep/vmrpalloc.o: ../h/vmem.h
archindep/vmrpalloc.o: ../h/vmem.sparc.h
archindep/vmrpalloc.o: archindep/vmrpalloc.c
archdep/getppte.o: ../h/a.out.h
archdep/getppte.o: ../h/conf.h
archdep/getppte.o: ../h/kernel.h
archdep/getppte.o: ../h/kernel.sparc.h
archdep/getppte.o: ../h/proc.h
archdep/getppte.o: ../h/q.h
archdep/getppte.o: ../h/select.h
archdep/getppte.o: ../h/types.h
archdep/getppte.o: ../h/vmem.h
archdep/getppte.o: ../h/vmem.sparc.h
archdep/getppte.o: archdep/getppte.c
archdep/hmmu.o: ../h/a.out.h
archdep/hmmu.o: ../h/conf.h
archdep/hmmu.o: ../h/kernel.h
archdep/hmmu.o: ../h/kernel.sparc.h
archdep/hmmu.o: ../h/mem.h
archdep/hmmu.o: ../h/openprom.h
archdep/hmmu.o: ../h/proc.h
archdep/hmmu.o: ../h/q.h
archdep/hmmu.o: ../h/select.h
archdep/hmmu.o: ../h/traps.h
archdep/hmmu.o: ../h/types.h
archdep/hmmu.o: ../h/vmem.h
archdep/hmmu.o: ../h/vmem.sparc.h
archdep/hmmu.o: archdep/hmmu.c
archdep/lmmu.o: ../h/asl.h
archdep/lmmu.o: ../h/vmem.h
archdep/lmmu.o: ../h/vmem.sparc.h
archdep/lmmu.o: archdep/lmmu.s
archdep/pmeg.o: ../h/a.out.h
archdep/pmeg.o: ../h/conf.h
archdep/pmeg.o: ../h/kernel.h
archdep/pmeg.o: ../h/kernel.sparc.h
archdep/pmeg.o: ../h/proc.h
archdep/pmeg.o: ../h/select.h
archdep/pmeg.o: ../h/types.h
archdep/pmeg.o: ../h/vmem.h
archdep/pmeg.o: ../h/vmem.sparc.h
archdep/pmeg.o: archdep/pmeg.c
archdep/pmegdump.o: ../h/a.out.h
archdep/pmegdump.o: ../h/conf.h
archdep/pmegdump.o: ../h/frame.h
archdep/pmegdump.o: ../h/kernel.h
archdep/pmegdump.o: ../h/kernel.sparc.h
archdep/pmegdump.o: ../h/mem.h
archdep/pmegdump.o: ../h/proc.h
archdep/pmegdump.o: ../h/select.h
archdep/pmegdump.o: ../h/sem.h
archdep/pmegdump.o: ../h/types.h
archdep/pmegdump.o: ../h/vmem.h
archdep/pmegdump.o: ../h/vmem.sparc.h
archdep/pmegdump.o: archdep/pmegdump.c
archdep/vac.o: ../h/enable.h
archdep/vac.o: archdep/vac.c
archdep/vacreg.o: ../h/asl.h
archdep/vacreg.o: ../h/vmem.h
archdep/vacreg.o: ../h/vmem.sparc.h
archdep/vacreg.o: archdep/vacreg.s
archdep/vminit.o: ../h/a.out.h
archdep/vminit.o: ../h/conf.h
archdep/vminit.o: ../h/kernel.h
archdep/vminit.o: ../h/kernel.sparc.h
archdep/vminit.o: ../h/mem.h
archdep/vminit.o: ../h/proc.h
archdep/vminit.o: ../h/q.h
archdep/vminit.o: ../h/select.h
archdep/vminit.o: ../h/types.h
archdep/vminit.o: ../h/vmem.h
archdep/vminit.o: ../h/vmem.sparc.h
archdep/vminit.o: archdep/vminit.c
fm/fmdump.o: ../h/a.out.h
fm/fmdump.o: ../h/conf.h
fm/fmdump.o: ../h/frame.h
fm/fmdump.o: ../h/kernel.h
fm/fmdump.o: ../h/kernel.sparc.h
fm/fmdump.o: ../h/mem.h
fm/fmdump.o: ../h/proc.h
fm/fmdump.o: ../h/select.h
fm/fmdump.o: ../h/sem.h
fm/fmdump.o: ../h/types.h
fm/fmdump.o: ../h/vmem.h
fm/fmdump.o: ../h/vmem.sparc.h
fm/fmdump.o: fm/fmdump.c
fm/fmtblinit.o: ../h/a.out.h
fm/fmtblinit.o: ../h/conf.h
fm/fmtblinit.o: ../h/frame.h
fm/fmtblinit.o: ../h/kernel.h
fm/fmtblinit.o: ../h/kernel.sparc.h
fm/fmtblinit.o: ../h/mem.h
fm/fmtblinit.o: ../h/proc.h
fm/fmtblinit.o: ../h/q.h
fm/fmtblinit.o: ../h/select.h
fm/fmtblinit.o: ../h/types.h
fm/fmtblinit.o: ../h/vmem.h
fm/fmtblinit.o: ../h/vmem.sparc.h
fm/fmtblinit.o: fm/fmtblinit.c
fm/fmutil.o: ../h/a.out.h
fm/fmutil.o: ../h/conf.h
fm/fmutil.o: ../h/frame.h
fm/fmutil.o: ../h/kernel.h
fm/fmutil.o: ../h/kernel.sparc.h
fm/fmutil.o: ../h/proc.h
fm/fmutil.o: ../h/select.h
fm/fmutil.o: ../h/types.h
fm/fmutil.o: ../h/vmem.h
fm/fmutil.o: ../h/vmem.sparc.h
fm/fmutil.o: fm/fmutil.c
fm/getframe.o: ../h/a.out.h
fm/getframe.o: ../h/conf.h
fm/getframe.o: ../h/frame.h
fm/getframe.o: ../h/kernel.h
fm/getframe.o: ../h/kernel.sparc.h
fm/getframe.o: ../h/proc.h
fm/getframe.o: ../h/q.h
fm/getframe.o: ../h/select.h
fm/getframe.o: ../h/types.h
fm/getframe.o: ../h/vmem.h
fm/getframe.o: ../h/vmem.sparc.h
fm/getframe.o: fm/getframe.c
fm/mapframe.o: ../h/a.out.h
fm/mapframe.o: ../h/conf.h
fm/mapframe.o: ../h/frame.h
fm/mapframe.o: ../h/kernel.h
fm/mapframe.o: ../h/kernel.sparc.h
fm/mapframe.o: ../h/proc.h
fm/mapframe.o: ../h/select.h
fm/mapframe.o: ../h/types.h
fm/mapframe.o: ../h/vmem.h
fm/mapframe.o: ../h/vmem.sparc.h
fm/mapframe.o: fm/mapframe.c
fm/pagereplace.o: ../h/a.out.h
fm/pagereplace.o: ../h/conf.h
fm/pagereplace.o: ../h/frame.h
fm/pagereplace.o: ../h/kernel.h
fm/pagereplace.o: ../h/kernel.sparc.h
fm/pagereplace.o: ../h/pgio.h
fm/pagereplace.o: ../h/proc.h
fm/pagereplace.o: ../h/select.h
fm/pagereplace.o: ../h/types.h
fm/pagereplace.o: ../h/vmem.h
fm/pagereplace.o: ../h/vmem.sparc.h
fm/pagereplace.o: fm/pagereplace.c
paging/nfspgio.o: ../h/a.out.h
paging/nfspgio.o: ../h/arp.h
paging/nfspgio.o: ../h/conf.h
paging/nfspgio.o: ../h/daemon.h
paging/nfspgio.o: ../h/dgram.h
paging/nfspgio.o: ../h/domain.h
paging/nfspgio.o: ../h/ether.h
paging/nfspgio.o: ../h/file.h
paging/nfspgio.o: ../h/frame.h
paging/nfspgio.o: ../h/fserver.h
paging/nfspgio.o: ../h/icmp.h
paging/nfspgio.o: ../h/ip.h
paging/nfspgio.o: ../h/ipreass.h
paging/nfspgio.o: ../h/kernel.h
paging/nfspgio.o: ../h/kernel.sparc.h
paging/nfspgio.o: ../h/mark.h
paging/nfspgio.o: ../h/net.h
paging/nfspgio.o: ../h/netconf.h
paging/nfspgio.o: ../h/netif.h
paging/nfspgio.o: ../h/network.h
paging/nfspgio.o: ../h/nfs.h
paging/nfspgio.o: ../h/nfspgio.h
paging/nfspgio.o: ../h/pgio.h
paging/nfspgio.o: ../h/proc.h
paging/nfspgio.o: ../h/q.h
paging/nfspgio.o: ../h/rfile.h
paging/nfspgio.o: ../h/rip.h
paging/nfspgio.o: ../h/route.h
paging/nfspgio.o: ../h/select.h
paging/nfspgio.o: ../h/snmpvars.h
paging/nfspgio.o: ../h/types.h
paging/nfspgio.o: ../h/udp.h
paging/nfspgio.o: ../h/vmem.h
paging/nfspgio.o: ../h/vmem.sparc.h
paging/nfspgio.o: paging/nfspgio.c
paging/pgio.o: ../h/a.out.h
paging/pgio.o: ../h/conf.h
paging/pgio.o: ../h/frame.h
paging/pgio.o: ../h/kernel.h
paging/pgio.o: ../h/kernel.sparc.h
paging/pgio.o: ../h/pgio.h
paging/pgio.o: ../h/proc.h
paging/pgio.o: ../h/select.h
paging/pgio.o: ../h/types.h
paging/pgio.o: ../h/vmem.h
paging/pgio.o: ../h/vmem.sparc.h
paging/pgio.o: paging/pgio.c
paging/tprotocol.o: ../h/a.out.h
paging/tprotocol.o: ../h/arp.h
paging/tprotocol.o: ../h/conf.h
paging/tprotocol.o: ../h/daemon.h
paging/tprotocol.o: ../h/dgram.h
paging/tprotocol.o: ../h/domain.h
paging/tprotocol.o: ../h/ether.h
paging/tprotocol.o: ../h/file.h
paging/tprotocol.o: ../h/frame.h
paging/tprotocol.o: ../h/fserver.h
paging/tprotocol.o: ../h/icmp.h
paging/tprotocol.o: ../h/ip.h
paging/tprotocol.o: ../h/ipreass.h
paging/tprotocol.o: ../h/kernel.h
paging/tprotocol.o: ../h/kernel.sparc.h
paging/tprotocol.o: ../h/mem.h
paging/tprotocol.o: ../h/net.h
paging/tprotocol.o: ../h/netconf.h
paging/tprotocol.o: ../h/netif.h
paging/tprotocol.o: ../h/network.h
paging/tprotocol.o: ../h/pgio.h
paging/tprotocol.o: ../h/proc.h
paging/tprotocol.o: ../h/rfile.h
paging/tprotocol.o: ../h/rip.h
paging/tprotocol.o: ../h/route.h
paging/tprotocol.o: ../h/select.h
paging/tprotocol.o: ../h/snmpvars.h
paging/tprotocol.o: ../h/tp.h
paging/tprotocol.o: ../h/types.h
paging/tprotocol.o: ../h/udp.h
paging/tprotocol.o: ../h/vmem.h
paging/tprotocol.o: ../h/vmem.sparc.h
paging/tprotocol.o: ../h/xpppgio.h
paging/tprotocol.o: paging/tprotocol.c
paging/xpppgio.o: ../h/a.out.h
paging/xpppgio.o: ../h/arp.h
paging/xpppgio.o: ../h/clock.h
paging/xpppgio.o: ../h/conf.h
paging/xpppgio.o: ../h/daemon.h
paging/xpppgio.o: ../h/dgram.h
paging/xpppgio.o: ../h/domain.h
paging/xpppgio.o: ../h/ether.h
paging/xpppgio.o: ../h/file.h
paging/xpppgio.o: ../h/frame.h
paging/xpppgio.o: ../h/fserver.h
paging/xpppgio.o: ../h/icmp.h
paging/xpppgio.o: ../h/ip.h
paging/xpppgio.o: ../h/ipreass.h
paging/xpppgio.o: ../h/kernel.h
paging/xpppgio.o: ../h/kernel.sparc.h
paging/xpppgio.o: ../h/mem.h
paging/xpppgio.o: ../h/net.h
paging/xpppgio.o: ../h/netconf.h
paging/xpppgio.o: ../h/netif.h
paging/xpppgio.o: ../h/network.h
paging/xpppgio.o: ../h/pgio.h
paging/xpppgio.o: ../h/proc.h
paging/xpppgio.o: ../h/rfile.h
paging/xpppgio.o: ../h/rip.h
paging/xpppgio.o: ../h/route.h
paging/xpppgio.o: ../h/select.h
paging/xpppgio.o: ../h/snmpvars.h
paging/xpppgio.o: ../h/tp.h
paging/xpppgio.o: ../h/types.h
paging/xpppgio.o: ../h/udp.h
paging/xpppgio.o: ../h/vmem.h
paging/xpppgio.o: ../h/vmem.sparc.h
paging/xpppgio.o: ../h/xpppgio.h
paging/xpppgio.o: paging/xpppgio.c
syscall/syscall.o: ../h/conf.h
syscall/syscall.o: ../h/kernel.h
syscall/syscall.o: ../h/kernel.sparc.h
syscall/syscall.o: ../h/mem.h
syscall/syscall.o: ../h/syscall.h
syscall/syscall.o: ../h/types.h
syscall/syscall.o: ../h/user_traps.h
syscall/syscall.o: ../h/vmem.h
syscall/syscall.o: ../h/vmem.sparc.h
syscall/syscall.o: syscall/syscall.c
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
