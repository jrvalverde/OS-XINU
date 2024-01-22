#
#  Top Level Makefile for VM Xinu 
#
#  Note: When you run "make all" we are assuming that you have already
#	 run make in all the subdirectories to make libx.a up-to-date
#	 If you want to be sure that everything is up-to-date, run
#	 "make complete" in this directory, it will run make in all
#	 the subdirectories.

KERNELADDR=0fd04000		

# Root Directory of VM Xinu
DIR	=	../..

# Machine specific things we must define
INSTR_TYPE	=	RISC
ARCH		=	SPARC
MODEL		=	SPARC_SLC
CODE_NAME	=	sun4c


HDIR	=	h
CDEFS	=	-D$(INSTR_TYPE) -D$(ARCH) -D$(MODEL) -D$(CODE_NAME) -DKERNEL
CFLAGS	=	-g -c -I${HDIR} ${CDEFS}
SFLAGS	=	-I${HDIR}
ASCPPFLAGS =	-I${HDIR} -DASM ${CDEFS}

AR	=	/bin/ar
CC	=	/bin/cc
LD	=	/bin/ld
MV	=	/bin/mv
LIB	=	${DIR}/lib
MAKETD	= 	../tools/maketd
LIBX	=	libx.a
RANLIBX	=	.${LIBX}


XOFILES =	sys/archdep/startup.o		\
		sys/archdep/initialize.o	\
		sys/archindep/test.o		\
		sys/conf.o
.c.o:
	${CC} ${CFLAGS} -o $@ $<
.s.o:
	/lib/cpp ${ASCPPFLAGS} $< > .tmp.s
	${CC} -c ${SFLAGS} -o $@ .tmp.s

all:	vmxinu ,nm

OFILES=	sys/xsys.o shell/xshell.o vm/xvm.o devs/xdevs.o	net/xnet.o misc/xmisc.o

pm.out: ${XOFILES} ${OFILES}
	$(LD) -e _start -N -X -T $(KERNELADDR) \
		${XOFILES} 	\
		${OFILES}	\
		${LIB}/libxc.a 	\
		-o ,pm.out
	${MV} ,pm.out pm.out

#pm.out: ${RANLIBX} ${XOFILES}
#	$(LD) -e _start -N -X -T $(KERNELADDR) \
#	${XOFILES} \
#	${LIBX} \
#	${LIB}/libxc.a \
#	${LIBX} \
#	${LIB}/libxc.a \
#	-o ,pm.out
#	${MV} ,pm.out pm.out


vmxinu: pm.out
	cp pm.out vmxinu
	strip vmxinu 
#	copy the image to a ftpboot directory
#	/usr/ucb/rcp vmxinu elizabeth:/tftpboot

,nm:	pm.out					
	nm -n pm.out > ,nm

${RANLIBX}: ${LIBX}
	ranlib ${LIBX}
	touch ${RANLIBX}

download:
	rcp b.out violin:/tftpboot/xinu/vmxinu
	rcp pm.out violin:/tftpboot/xinu/vmxinu.g

clean:
	find . -name '*'.o -exec \rm {} \;

complete: sys_dir vm_dir devs_dir shell_dir misc_dir net_dir all

sys_dir:
	cd sys; make; cd ..

vm_dir:
	cd vm; make; cd ..

devs_dir:
	cd devs; make; cd ..

net_dir:
	cd net; make; cd ..

shell_dir:
	cd shell; make; cd ..

misc_dir:
	cd misc; make; cd ..

altall:	alt.a.out ,nm

allaltall: xdotofiles alt.a.out ,nm

xdotofiles:
	cd devs; make altall; cd ..
	cd misc; make altall; cd ..
	cd net; make altall; cd ..
	cd shell; make altall; cd ..
	cd sys; make altall; cd ..
	cd vm; make altall; cd ..

alt.a.out: ${XOFILES}
	$(LD) -e _start -N -X -T $(KERNELADDR) \
		${XOFILES} \
		${BIGOFILES}\
		${LIB}/libxc.a \
		-o ,pm.out
	${MV} ,pm.out pm.out
	cp pm.out b.out
	strip b.out
	dd if=b.out of=a.out ibs=32 skip=1

subdepend:
	for DIR in sys vm devs net shell misc; do \
		cd $DIR; make depend; cd ..; \
	done

depend:
	${MAKETD} -d -osys/archdep ${CDEFS} -I${HDIR} sys/archdep/initialize.c > makedep
	${MAKETD} -d -osys/archindep ${CDEFS} -I${HDIR} sys/archindep/test.c >> makedep
	${MAKETD} -d -oconf ${CDEFS} -I${HDIR} sys/conf.c >> makedep
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
sys/archdep/initialize.o: h/a.out.h
sys/archdep/initialize.o: h/arp.h
sys/archdep/initialize.o: h/conf.h
sys/archdep/initialize.o: h/daemon.h
sys/archdep/initialize.o: h/dgram.h
sys/archdep/initialize.o: h/domain.h
sys/archdep/initialize.o: h/ether.h
sys/archdep/initialize.o: h/file.h
sys/archdep/initialize.o: h/frame.h
sys/archdep/initialize.o: h/fserver.h
sys/archdep/initialize.o: h/icmp.h
sys/archdep/initialize.o: h/io.h
sys/archdep/initialize.o: h/ip.h
sys/archdep/initialize.o: h/ipreass.h
sys/archdep/initialize.o: h/kernel.h
sys/archdep/initialize.o: h/kernel.sparc.h
sys/archdep/initialize.o: h/mem.h
sys/archdep/initialize.o: h/net.h
sys/archdep/initialize.o: h/netconf.h
sys/archdep/initialize.o: h/netif.h
sys/archdep/initialize.o: h/network.h
sys/archdep/initialize.o: h/proc.h
sys/archdep/initialize.o: h/q.h
sys/archdep/initialize.o: h/rfile.h
sys/archdep/initialize.o: h/rip.h
sys/archdep/initialize.o: h/route.h
sys/archdep/initialize.o: h/select.h
sys/archdep/initialize.o: h/sem.h
sys/archdep/initialize.o: h/sleep.h
sys/archdep/initialize.o: h/snmpvars.h
sys/archdep/initialize.o: h/tty.h
sys/archdep/initialize.o: h/types.h
sys/archdep/initialize.o: h/udp.h
sys/archdep/initialize.o: h/vmem.h
sys/archdep/initialize.o: h/vmem.sparc.h
sys/archdep/initialize.o: sys/archdep/initialize.c
sys/archindep/test.o: h/a.out.h
sys/archindep/test.o: h/arp.h
sys/archindep/test.o: h/conf.h
sys/archindep/test.o: h/daemon.h
sys/archindep/test.o: h/dgram.h
sys/archindep/test.o: h/domain.h
sys/archindep/test.o: h/ether.h
sys/archindep/test.o: h/file.h
sys/archindep/test.o: h/fserver.h
sys/archindep/test.o: h/icmp.h
sys/archindep/test.o: h/ip.h
sys/archindep/test.o: h/ipreass.h
sys/archindep/test.o: h/kernel.h
sys/archindep/test.o: h/kernel.sparc.h
sys/archindep/test.o: h/mem.h
sys/archindep/test.o: h/net.h
sys/archindep/test.o: h/netconf.h
sys/archindep/test.o: h/netif.h
sys/archindep/test.o: h/network.h
sys/archindep/test.o: h/proc.h
sys/archindep/test.o: h/rfile.h
sys/archindep/test.o: h/rip.h
sys/archindep/test.o: h/route.h
sys/archindep/test.o: h/select.h
sys/archindep/test.o: h/snmpvars.h
sys/archindep/test.o: h/tp.h
sys/archindep/test.o: h/types.h
sys/archindep/test.o: h/udp.h
sys/archindep/test.o: h/vmem.h
sys/archindep/test.o: h/vmem.sparc.h
sys/archindep/test.o: h/xpppgio.h
sys/archindep/test.o: sys/archindep/test.c
conf/conf.o: h/conf.h
conf/conf.o: sys/conf.c
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
