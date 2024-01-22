#  Makefile for PC Xinu to compile on a Solais X86 PC.
#

DEFS	= -DBSDURG
INCLUDE = -I../h
CFLAGS	= -c -traditional ${DEFS} ${INCLUDE}
SDEFS	= -I../h

CC      =       /usr/local/gnu/gcc
CPP     =       /usr/local/gnu/gcc -E
AS      =       /usr/local/gnu/gas-2.5.2
LD      =       /usr/ccs/bin/ld
LIB     =       ../lib
AWK	=	awk
XINU    =       ./pcxinu

# Amount to move loaded image down in memory
BRELOC  =	0x150000
BOOTPLOC=	0x150000

MON =   monitor.c       monarp.c        monbootp.c      \
	monip.c         monnet.c        monudp.c        mongpq.c        \
	eeintr.c        eewrite.c       eeinit.c        eedemux.c       \
	eewstrt.c       eerom.c         eecmd.c         monboot.c       \
	montimer.c

COM =	comcntl.c	comgetc.c	comiin.c	cominit.c	\
	cominput.c	comoutput.c	comread.c	comintr.c

CON =	kbmcntl.c	kbmgetc.c	kbmiin.c	kbminit.c	\
	kbminput.c	kbmoutput.c	kbscan.c

DEBUG =	arpdump.c	hexdump.c	icmpdump.c	igmpdump.c	\
	ipdump.c	ospfdump.c	pdump.c		tcpdump.c	\
	udpdump.c

DGRAM =	dgalloc.c	dgclose.c	dgcntl.c	dgdump.c	\
	dginit.c	dgmcntl.c	dgmopen.c	\
	dgread.c	dgwrite.c

ARP =   arp_in.c        arpadd.c        arpalloc.c      arpdq.c         \
	arpfind.c       arpinit.c       arpprint.c      arpqsend.c      \
	arpsend.c       arptimer.c

IP =    ip_in.c         ipdbc.c         ipdoopts.c      ipdstopts.c     \
	ipfadd.c        ipfcons.c       ipfhcopy.c      ipfinit.c       \
	ipfjoin.c       ipfsend.c       ipftimer.c      ipgetp.c        \
	iph2net.c       ipnet2h.c       ipproc.c        ipputp.c        \
	ipreass.c       ipredirect.c    ipsend.c                        \
	isbrc.c         rtadd.c         rtdel.c         rtdump.c        \
	rtfree.c        rtget.c         rthash.c        rtinit.c        \
	rtnew.c         rttimer.c

ICMP =	icerrok.c	icmp.c		icmp_in.c	icredirect.c	\
	icsetbuf.c	icsetdata.c	icsetsrc.c

IGMP =	hgadd.c		hgarpadd.c	hgarpdel.c	\
	hginit.c	hgjoin.c	\
	hgleave.c	igmp.c		igmp_in.c	hgprint.c	\
	hglookup.c	igmp_update.c	hgrand.c	igmp_settimers.c

NM =	nm_in.c		nm_init.c	stlookup.c	stnew.c

TM =	tcptimer.c	tmclear.c	tmleft.c	tmset.c		\
	tqdump.c

TCP =	tcp_in.c	tcpabort.c	tcpacked.c	tcpackit.c	\
	tcpcksum.c	tcpclosed.c	tcpclosewait.c	tcpclosing.c	\
	tcpdata.c	tcpdemux.c	tcpdodat.c	tcpestablished.c\
	tcpfin1.c	tcpfin2.c	tcph2net.c	tcphowmuch.c	\
	tcpidle.c	tcpinp.c	tcpiss.c	tcpkick.c	\
	tcpkilltimers.c	tcplastack.c	tcplisten.c	tcpnet2h.c	\
	tcpok.c		tcpopts.c	tcpostate.c	tcpout.c	\
	tcppersist.c	tcpreset.c	tcprexmt.c	\
	tcprmss.c	tcprtt.c	tcprwindow.c	tcpsend.c	\
	tcpsmss.c	tcpsndlen.c	\
	tcpswindow.c	tcpswitch.c	tcpsync.c	tcpsynrcvd.c	\
	tcpsynsent.c	tcptimewait.c	tcpwait.c	tcpwakeup.c	\
	tcpwinit.c	tcpxmit.c	tfcoalesce.c	tfinsert.c	\
	${TM}

OSPF =	ospf_hin.c	ospf_hsend.c	ospf_in.c	fcksum.c	\
	ospfcheck.c	if_elect.c	ospfhello.c	ospfhtmpl.c	\
	ospfifinit.c	ospfinit.c	ospf.c		ospfnet2h.c	\
	ospftimer.c	nb_add.c	nb_switch.c	nb_reform.c	\
	nb_aok.c	dd_queue.c	dd_xmit.c	lss_build.c	\
	ospf_ddin.c	ospfddtmpl.c	nb_rexmt.c	nb_clearl.c	\
	nb_makel.c	nb_mismatch.c	lsa_xmit.c	if_elect1.c	\
	ddi_exstart.c	ddi_exchng.c	lsr_in.c	db_lookup.c	\
	db_update.c	ddi_full.c	db_new.c	db_init.c	\
	lsa_add.c	lsa_send.c	ospflstmpl.c	lsu_in.c	\
	lsack_in.c	db_nlink.c	db_rlink.c	db_resync.c	\
	lsr_queue.c	lsr_add.c	lsr_xmit.c	ospflsrtmpl.c	\
	lsr_check.c

UDP =	udp_in.c	udpcksum.c	udph2net.c	udpnet2h.c	\
	udpnxtp.c	udpsend.c	upalloc.c	

NET =   dnparse.c       dot2ip.c        ethmatch.c      \
	getaddr.c       \
	getname.c       getnet.c        getutim.c       initgate.c      \
	inithost.c      ip2dot.c        ip2name.c       local_out.c     \
	name2ip.c       netdump.c       netmask.c       netmatch.c      \
	netnum.c        netstart.c      netwrite.c      ni_in.c         \
	othinit.c       othwrite.c      rarp_in.c       rarpsend.c      \
	setmask.c       slowtimer.c     ethmcast.c      bootp.c

NA =	echod.c		egp.c		fingerd.c	lfing.c		\
	ripadd.c	ripcheck.c	ripifset.c	ripin.c		\
	ripmetric.c	ripok.c		ripout.c	riprecv.c	\
	riprepl.c	ripsend.c	ripstart.c	rwho.c		\
	rwhod.c		rwhoind.c	snmpd.c		udpecho.c

RFS =	mount.c		rfalloc.c	rfclose.c	rfcntl.c	\
	rfdump.c	rfgetc.c	rfinit.c	rfio.c		\
	rfmkpac.c	rfopen.c	rfputc.c	rfread.c	\
	rfseek.c	rfsend.c	rfwrite.c	unmount.c	

SYS =	blkcmp.c	blkequ.c	main.c		stacktrace.c	\
	chprio.c	ckmode.c	clkinit.c	close.c		\
	conf.c		control.c	create.c	\
	evec.c		freebuf.c	freemem.c	\
	getbuf.c	getc.c		getitem.c	\
	getmem.c	getpid.c	getprio.c	getstk.c	\
	gettime.c	gpq.c		i386.c		init.c		\
	insert.c	insertd.c	ioerr.c		\
	ionull.c	kill.c		kprintf.c	\
	kputc.c		mark.c		mkpool.c	\
	naminit.c	nammap.c	namopen.c	namrepl.c	\
	newqueue.c	open.c		panic.c		pcount.c	\
	pcreate.c	pdelete.c	pinit.c		poolinit.c	\
	preceive.c	preset.c	printq.c	psend.c		\
	ptclear.c	putc.c		queue.c		read.c		\
	ready.c		receive.c	recvclr.c	recvtim.c	\
	remove.c	rename.c	resched.c	resume.c	\
	scount.c	screate.c	sdelete.c	seek.c		\
	send.c		sendf.c		setdev.c	setnok.c	\
	signal.c	signaln.c	sleep.c		sleep10.c	\
	sleep100.c	sreset.c	suspend.c	\
	unsleep.c	userret.c	wait.c		wakeup.c	\
	write.c		xdone.c		syslog.c	getuser.c	\
	getuid.c

TCPD =	tcballoc.c	tcbdealloc.c	tcpbind.c	\
	tcpclose.c	tcpcntl.c	tcpcon.c	tcpgetc.c	\
	tcpgetdata.c	tcpgetspace.c	tcpinit.c	tcplq.c		\
	tcpmcntl.c	tcpmopen.c	tcpnxtp.c	tcpputc.c	\
	tcpread.c	tcpserver.c	\
	tcpstat.c	tcpuopt.c	tcpwr.c		tcpwrite.c

TTY =	ttyalloc.c	ttycntl.c	ttygetc.c	ttyiin.c	\
	ttyinit.c	ttynew.c	ttyopen.c	ttyputc.c	\
	ttyread.c	ttywrite.c	ttyoin.c

SNMP =	a1rwint.c	a1rwlen.c	a1rwoid.c	a1rwval.c	\
	mksnmp.c	sae_findn.c	sae_get.c	sae_getf.c	\
	sae_getn.c	sae_match.c	sae_set.c	sat_findn.c	\
	sat_get.c	sat_getf.c	sat_getn.c	sat_match.c	\
	sat_set.c	sif_get.c	sif_getf.c	sif_getn.c	\
	sif_match.c	sif_set.c	sna2b.c		snb2a.c		\
	snclient.c	snerr.c		snfreebl.c	snhash.c	\
	sninit.c	snleaf.c	snmib.c		snoip.c		\
	snparse.c	snprint.c	snrslv.c	sntable.c	\
	srt_findn.c	srt_get.c	srt_getf.c	srt_getn.c	\
	srt_match.c	srt_set.c	stc_findn.c	stc_get.c	\
	stc_getf.c	stc_getn.c	stc_match.c	stc_set.c	

ETHUE =	ue_intr.c	ue_write.c	ue_init.c	ue_cntl.c	\
	ue_demux.c	ue_wstrt.c

ETHEE = ee_intr.c	ee_write.c	ee_init.c	ee_cntl.c	\
	ee_demux.c	ee_wstrt.c	ee_rom.c	ee_cmd.c	\
	ee_mcast.c	ee_read.c

SRC = ${ARP} ${COM} ${CON} ${DGRAM} ${ICMP} ${IGMP} ${IP} ${NA} ${NM}	\
	${NET}	${RFS} ${SNMP} ${SYS} ${TCPD} ${TCP} ${TTY} ${UDP}	\
	${MON} ${DEBUG} ${OSPF}

MONOBJ= monitor.o       monarp.o        monbootp.o      \
	monip.o         monnet.o        monudp.o        mongpq.o        \
	eeintr.o        eewrite.o       eeinit.o        eedemux.o       \
	eewstrt.o       eerom.o         eecmd.o         monboot.o       \
	montimer.o

COMOBJ = comcntl.o	comgetc.o	comiin.o	cominit.o	\
	cominput.o	comoutput.o	comread.o		\
	comintr.o

CONOBJ = kbmcntl.o	kbmgetc.o	kbmiin.o	kbminit.o	\
	kbminput.o	kbmoutput.o	kbscan.o	
	

DEBUGOBJ = arpdump.o	hexdump.o	icmpdump.o	igmpdump.o	\
	ipdump.o	ospfdump.o	pdump.o		tcpdump.o	\
	udpdump.o

DGRAMOBJ = dgalloc.o	dgclose.o	dgcntl.o	dgdump.o	\
	dginit.o	dgmcntl.o	dgmopen.o	\
	dgread.o	dgwrite.o

ARPOBJ = arp_in.o       arpadd.o        arpalloc.o      arpdq.o         \
        arpfind.o       arpinit.o       arpprint.o      arpqsend.o      \
        arpsend.o       arptimer.o

IPOBJ = ip_in.o         ipdbc.o         ipdoopts.o      ipdstopts.o     \
	ipfadd.o        ipfcons.o       ipfhcopy.o      ipfinit.o       \
	ipfjoin.o       ipfsend.o       ipftimer.o      ipgetp.o        \
	iph2net.o       ipnet2h.o       ipproc.o        ipputp.o        \
	ipreass.o       ipredirect.o    ipsend.o        \
	isbrc.o         rtadd.o         rtdel.o         rtdump.o        \
	rtfree.o        rtget.o         rthash.o        rtinit.o        \
	rtnew.o         rttimer.o

ICOBJ = icerrok.o	icmp.o		icmp_in.o	icredirect.o	\
	icsetbuf.o	icsetdata.o	icsetsrc.o

IGOBJ =	hgadd.o		hgarpadd.o	hgarpdel.o	\
	hginit.o	hgjoin.o	\
	hgleave.o	igmp.o		igmp_in.o	hgprint.o	\
	hglookup.o	igmp_update.o	hgrand.o	igmp_settimers.o

TMOBJ = tcptimer.o	tmclear.o	tmleft.o	tmset.o		\
	tqdump.o

TCPOBJ = tcp_in.o	tcpabort.o	tcpacked.o	tcpackit.o	\
	tcpcksum.o	tcpclosed.o	tcpclosewait.o	tcpclosing.o	\
	tcpdata.o	tcpdemux.o	tcpdodat.o	tcpestablished.o\
	tcpfin1.o	tcpfin2.o	tcph2net.o	tcphowmuch.o	\
	tcpidle.o	tcpinp.o	tcpiss.o	tcpkick.o	\
	tcpkilltimers.o	tcplastack.o	tcplisten.o	tcpnet2h.o	\
	tcpok.o		tcpopts.o	tcpostate.o	tcpout.o	\
	tcppersist.o	tcpreset.o	tcprexmt.o	\
	tcprmss.o	tcprtt.o	tcprwindow.o	tcpsend.o	\
	tcpsmss.o	tcpsndlen.o	\
	tcpswindow.o	tcpswitch.o	tcpsync.o	tcpsynrcvd.o	\
	tcpsynsent.o	tcptimewait.o	tcpwait.o	tcpwakeup.o	\
	tcpwinit.o	tcpxmit.o	tfcoalesce.o	tfinsert.o	\
	${TMOBJ}

OSPFOBJ = ospf_hin.o	ospf_hsend.o	ospf_in.o	fcksum.o	\
	ospfcheck.o	if_elect.o	ospfhello.o	ospfhtmpl.o	\
	ospfifinit.o	ospfinit.o	ospf.o		ospfnet2h.o	\
	ospftimer.o	nb_add.o	nb_switch.o	nb_reform.o	\
	nb_aok.o	dd_queue.o	dd_xmit.o	lss_build.o	\
	ospf_ddin.o	ospfddtmpl.o	nb_rexmt.o	nb_clearl.o	\
	nb_makel.o	lsa_xmit.o	nb_mismatch.o	if_elect1.o	\
	ddi_exstart.o	ddi_exchng.o	lsr_in.o	db_lookup.o	\
	db_update.o	ddi_full.o	db_new.o	db_init.o	\
	lsa_add.o	lsa_send.o	ospflstmpl.o	lsu_in.o	\
	lsack_in.o	db_nlink.o	db_rlink.o	db_resync.o	\
	lsr_queue.o	lsr_add.o	lsr_xmit.o	ospflsrtmpl.o	\
	lsr_check.o

UDPOBJ = udp_in.o	udpcksum.o	udph2net.o	udpnet2h.o	\
	udpnxtp.o	udpsend.o	upalloc.o	

NETOBJ = dnparse.o      dot2ip.o        getaddr.o       ethmatch.o      \
	getname.o       getnet.o        getutim.o       initgate.o      \
	inithost.o      ip2dot.o        ip2name.o       local_out.o     \
	name2ip.o       netdump.o       netmask.o       netmatch.o      \
	netnum.o        netstart.o      netwrite.o      ni_in.o         \
	othinit.o       othwrite.o      rarp_in.o       rarpsend.o      \
	setmask.o       slowtimer.o     ethmcast.o      bootp.o

NAOBJ = echod.o		egp.o		fingerd.o	lfing.o		\
	ripadd.o	ripcheck.o	ripifset.o	ripin.o		\
	ripmetric.o	ripok.o		ripout.o	riprecv.o	\
	riprepl.o	ripsend.o	ripstart.o	rwho.o		\
	rwhod.o		rwhoind.o	snmpd.o		udpecho.o

NMOBJ =	nm_in.o		nm_init.o	stlookup.o	stnew.o

RFSOBJ = mount.o	rfalloc.o	rfclose.o	rfcntl.o	\
	rfdump.o	rfgetc.o	rfinit.o	rfio.o		\
	rfmkpac.o	rfopen.o	rfputc.o	rfread.o	\
	rfseek.o	rfsend.o	rfwrite.o	unmount.o	

SYSOBJ = blkcmp.o	blkequ.o	main.o		stacktrace.o 	\
	chprio.o	ckmode.o	clkinit.o	close.o		\
	conf.o		control.o	create.o	\
	evec.o		freebuf.o	freemem.o	\
	getbuf.o	getc.o		getitem.o	\
	getmem.o	getpid.o	getprio.o	getstk.o	\
	gettime.o	gpq.o		i386.o		init.o		\
	insert.o	insertd.o	ioerr.o		\
	ionull.o	kill.o		kprintf.o	\
	kputc.o		mark.o		mkpool.o	\
	naminit.o	nammap.o	namopen.o	namrepl.o	\
	newqueue.o	open.o		panic.o		pcount.o	\
	pcreate.o	pdelete.o	pinit.o		poolinit.o	\
	preceive.o	preset.o	printq.o	psend.o		\
	ptclear.o	putc.o		queue.o		read.o		\
	ready.o		receive.o	recvclr.o	recvtim.o	\
	remove.o	rename.o	resched.o	resume.o	\
	scount.o	screate.o	sdelete.o	seek.o		\
	send.o		sendf.o		setdev.o	setnok.o	\
	signal.o	signaln.o	sleep.o		sleep10.o	\
	sleep100.o	sreset.o	suspend.o	\
	unsleep.o	userret.o	wait.o		wakeup.o	\
	write.o		xdone.o		syslog.o	getuid.o	\
	getuser.o

TCPDOBJ = tcballoc.o	tcbdealloc.o	tcpbind.o	\
	tcpclose.o	tcpcntl.o	tcpcon.o	tcpgetc.o	\
	tcpgetdata.o	tcpgetspace.o	tcpinit.o	tcplq.o		\
	tcpmcntl.o	tcpmopen.o	tcpnxtp.o	tcpputc.o	\
	tcpread.o	tcpserver.o	\
	tcpstat.o	tcpuopt.o	tcpwr.o		tcpwrite.o

TTYOBJ = ttyalloc.o	ttycntl.o	ttygetc.o	ttyiin.o	\
	ttyinit.o	ttynew.o	ttyopen.o	ttyputc.o	\
	ttyread.o	ttywrite.o	ttyoin.o

SNOBJ = a1rwint.o	a1rwlen.o	a1rwoid.o	a1rwval.o	\
	mksnmp.o	sae_findn.o	sae_get.o	sae_getf.o	\
	sae_getn.o	sae_match.o	sae_set.o	sat_findn.o	\
	sat_get.o	sat_getf.o	sat_getn.o	sat_match.o	\
	sat_set.o	sif_get.o	sif_getf.o	sif_getn.o	\
	sif_match.o	sif_set.o	sna2b.o		snb2a.o		\
	snclient.o	snerr.o		snfreebl.o	snhash.o	\
	sninit.o	snleaf.o	snmib.o		snoip.o		\
	snparse.o	snprint.o	snrslv.o	sntable.o	\
	srt_findn.o	srt_get.o	srt_getf.o	srt_getn.o	\
	srt_match.o	srt_set.o	stc_findn.o	stc_get.o	\
	stc_getf.o	stc_getn.o	stc_match.o	stc_set.o

ETHUEOBJ = ue_intr.o	ue_write.o	ue_init.o	ue_cntl.o	\
	ue_demux.o	ue_wstrt.o

ETHEEOBJ = ee_intr.o	ee_write.o	ee_init.o	ee_cntl.o	\
	ee_demux.o	ee_wstrt.o	ee_mcast.o	\
	ee_rom.o	ee_cmd.o	ee_read.o

XOBJ = startup.o initialize.o intr.o clkint.o ctxsw.o sh.o

OBJ = ${ARPOBJ} ${COMOBJ} ${CONOBJ} ${DGRAMOBJ} ${ICOBJ} ${IGOBJ} 	\
	${IPOBJ} ${NAOBJ} ${NMOBJ}  ${RFSOBJ} ${SNOBJ} ${MONOBJ}	\
	${SYSOBJ} ${TCPDOBJ} ${TCPOBJ} ${TTYOBJ} ${UDPOBJ} ${DEBUGOBJ}	\
	${ETHEEOBJ} ${ETHALOBJ} ${NETOBJ} \
	moncksum.o moneeint.o monclkint.o montftp.o comint.o kbmint.o 	\
	ee_int.o cksum.o

all: xinu FRC

xinu: Makefile xinu.mapfile Configuration ${OBJ} ${XOBJ} ${LIB}/libxc.a
	$(LD) -dn -M xinu.mapfile -e start ${XOBJ} ${OBJ} \
	${LIB}/libxc.a -o ${XINU}.elf
	./mkboot ${XINU}.elf ${XINU}
	rcp ${XINU} lin@balan.cs:/tftpboot/pcxinu.emu

install: xinu FRC
	cp xinu /floppy/xinu

clean: FRC
	rm -rf *.o *.errs *.bak *.a core errs ${XINU}* tags typescript* version
	echo '0' > vn
	(cd ../shell; make clean)

depend: makedep
	echo '/^# DO NOT DELETE THIS LINE/+1,$$d' >eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile Makefile.bak
	ex - Makefile < eddep
	rm eddep makedep makedep1 makedeperrs
	(cd ../shell; make depend)

CFILES	= ../arp/*.c ../com/*.c ../kbm/*.c ../dgram/*.c ../icmp/*.c	\
	../igmp/*.c ../ip/*.c ../net/*.c ../netmon/*.c ../netapp/*.c	\
        ../rfs/*.c ../snmp/*.c ../sys/*.c ../tcp/*.c ../tcpd/*.c	\
        ../tty/*.c ../udp/*.c ../ospf/*.c ../debug/*.c ../ether/ee/*.c 	\
	../mon/*.c

makedep: FRC
	@cat /dev/null >makedep
	-(for i in ${CFILES} ; do \
		${CPP} -M ${DEFS} -I../h ./$$i >> makedep; done) \
		2>makedeperrs
	sed -e 's,^\(.*\)\.o:,\1.o:,' makedep > makedep1
	${AWK} ' { if ($$1 != prev) { print rec; rec = $$0; prev = $$1; } \
		else { if (length(rec $$3) > 78) { print rec; rec = $$0; } \
			else rec = rec " " $$3 } } \
		END { print rec } ' makedep1 > makedep
	@cat makedeperrs
	@(if [ -s makedeperrs ]; then false; fi)

xdepend: FRC
	maketd ${DEFS} -I../h ${CFILES}
	(cd ../shell; make depend)

#
# the directory list below should match the "SRCDIR" list below, + ../h
#
tags: FRC
	ctags ${SRC}

../h/conf.h conf.c: Configuration
	../config/config; mv conf.c ../sys

sh.o: FRC
	(cd ../shell; make)

intr.o: ../sys/intr.S
	${CPP} ${SDEFS} ../sys/intr.S | ${AS} -o intr.o

cksum.o: ../net/cksum.S
	${CPP} ${SDEFS} ../net/cksum.S | ${AS} -o cksum.o

clkint.o: ../sys/clkint.S
	${CPP} ${SDEFS} ../sys/clkint.S | ${AS} -o clkint.o

ctxsw.o: ../sys/ctxsw.S
	${CPP} ${SDEFS} ../sys/ctxsw.S | ${AS} -o ctxsw.o

startup.o: ../sys/startup.S Makefile
	${CPP} ${SDEFS} -DBRELOC=${BRELOC} -DBOOTPLOC=${BOOTPLOC} \
	-DLINUX_XINU ../sys/startup.S | ${AS} -o startup.o 

head.o: head.S
	${CPP} ${SDEFS} head.S | ${AS} -o head.o

moneeint.o: ../mon/moneeint.S
	${CPP} ${SDEFS} ../mon/moneeint.S | ${AS} -o moneeint.o

moncksum.o: ../mon/moncksum.S
	${CPP} ${SDEFS} ../mon/moncksum.S | ${AS} -o moncksum.o

monclkint.o: ../mon/monclkint.S
	${CPP} ${SDEFS} ../mon/monclkint.S | ${AS} -o monclkint.o

montftp.o: ../mon/montftp.c
	${CC} ${CFLAGS} -DBOOTPLOC=${BOOTPLOC} ../mon/montftp.c -o montftp.o

comint.o: ../com/comint.S
	${CPP} ${SDEFS} ../com/comint.S | ${AS} -o comint.o

kbmint.o: ../kbm/kbmint.S
	${CPP} ${SDEFS} ../kbm/kbmint.S | ${AS} -o kbmint.o

ue_int.o: ../ether/ue/ue_int.S
	${CPP} ${SDEFS} ../ether/ue/ue_int.S | ${AS} -o ue_int.o

ee_int.o: ../ether/ee/ee_int.S
	${CPP} ${SDEFS} ../ether/ee/ee_int.S | ${AS} -o ee_int.o

initialize.o: $(OBJ) startup.o 
	sh mkvers.sh
	${CC} -c ${CFLAGS} -DVERSION=\""`cat version`"\" ../sys/initialize.c

${MONOBJ}:
	${CC} ${CFLAGS} ../mon/`basename $@ .o`.[c]
${ARPOBJ}:
	 ${CC} ${CFLAGS} ../arp/`basename $@ .o`.[c]
${COMOBJ}:
	 ${CC} ${CFLAGS} ../com/`basename $@ .o`.[c]
${CONOBJ}:
	 ${CC} ${CFLAGS} ../kbm/`basename $@ .o`.[c]
${DEBUGOBJ}:
	 ${CC} ${CFLAGS} ../debug/`basename $@ .o`.[c]
${DGRAMOBJ}:
	 ${CC} ${CFLAGS} ../dgram/`basename $@ .o`.[c]
${ICOBJ}:
	 ${CC} ${CFLAGS} ../icmp/`basename $@ .o`.[c]
${IGOBJ}:
	 ${CC} ${CFLAGS} ../igmp/`basename $@ .o`.[c]
${IPOBJ}:
	 ${CC} ${CFLAGS} ../ip/`basename $@ .o`.[c]
${NAOBJ}:
	 ${CC} ${CFLAGS} ../netapp/`basename $@ .o`.[c]
${NMOBJ}:
	 ${CC} ${CFLAGS} ../netmon/`basename $@ .o`.[c]
${NETOBJ}:
	 ${CC} ${CFLAGS} ../net/`basename $@ .o`.[c]
${OSPFOBJ}:
	 ${CC} ${CFLAGS} ../ospf/`basename $@ .o`.[c]
${RFSOBJ}:
	 ${CC} ${CFLAGS} ../rfs/`basename $@ .o`.[c]
${SNOBJ}:
	 ${CC} ${CFLAGS} ../snmp/`basename $@ .o`.[c]
${SYSOBJ} intialize.o :
	 ${CC} ${CFLAGS} ../sys/`basename $@ .o`.[c]
${TCPDOBJ}:
	 ${CC} ${CFLAGS} ../tcpd/`basename $@ .o`.[c]
${TCPOBJ}:
	 ${CC} ${CFLAGS} ../tcp/`basename $@ .o`.[c]
${TTYOBJ}:
	 ${CC} ${CFLAGS} ../tty/`basename $@ .o`.[c]
${UDPOBJ}:
	 ${CC} ${CFLAGS} ../udp/`basename $@ .o`.[c]
${ETHEEOBJ}:
	 ${CC} ${CFLAGS} ../ether/ee/`basename $@ .o`.[c]

FRC:
	
# DO NOT DELETE THIS LINE - make depend DEPENDS ON IT

arp_in.o: ./../arp/arp_in.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
arpadd.o: ./../arp/arpadd.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
arpalloc.o: ./../arp/arpalloc.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
arpdq.o: ./../arp/arpdq.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
arpfind.o: ./../arp/arpfind.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
arpinit.o: ./../arp/arpinit.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
arpprint.o: ./../arp/arpprint.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
arpqsend.o: ./../arp/arpqsend.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
arpsend.o: ./../arp/arpsend.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
arptimer.o: ./../arp/arptimer.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
comcntl.o: ./../com/comcntl.c ../h/conf.h ../h/kernel.h
comgetc.o: ./../com/comgetc.c ../h/conf.h ../h/kernel.h ../h/io.h
comiin.o: ./../com/comiin.c ../h/conf.h ../h/kernel.h ../h/tty.h \
 ../h/com.h
cominit.o: ./../com/cominit.c ../h/conf.h ../h/kernel.h ../h/tty.h \
 ../h/com.h
cominput.o: ./../com/cominput.c ../h/conf.h ../h/kernel.h ../h/tty.h \
 ../h/io.h
comintr.o: ./../com/comintr.c ../h/conf.h ../h/kernel.h ../h/tty.h \
 ../h/com.h
comoutput.o: ./../com/comoutput.c ../h/conf.h ../h/kernel.h ../h/tty.h \
 ../h/com.h
comread.o: ./../com/comread.c ../h/conf.h ../h/kernel.h ../h/io.h
kbmcntl.o: ./../kbm/kbmcntl.c ../h/conf.h ../h/kernel.h
kbmgetc.o: ./../kbm/kbmgetc.c ../h/conf.h ../h/kernel.h ../h/io.h
kbmiin.o: ./../kbm/kbmiin.c ../h/conf.h ../h/kernel.h ../h/io.h \
 ../h/tty.h ../h/kbd.h
kbminit.o: ./../kbm/kbminit.c ../h/conf.h ../h/kernel.h ../h/tty.h \
 ../h/io.h ../h/kbm.h
kbminput.o: ./../kbm/kbminput.c ../h/conf.h ../h/kernel.h ../h/tty.h \
 ../h/io.h
kbmoutput.o: ./../kbm/kbmoutput.c ../h/conf.h ../h/kernel.h ../h/io.h \
 ../h/tty.h
kbscan.o: ./../kbm/kbscan.c ../h/kernel.h ../h/tty.h ../h/kbd.h
dgalloc.o: ./../dgram/dgalloc.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
dgclose.o: ./../dgram/dgclose.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
dgcntl.o: ./../dgram/dgcntl.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
dgdump.o: ./../dgram/dgdump.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
dginit.o: ./../dgram/dginit.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
dgmcntl.o: ./../dgram/dgmcntl.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
dgmopen.o: ./../dgram/dgmopen.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
dgread.o: ./../dgram/dgread.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
dgwrite.o: ./../dgram/dgwrite.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
icerrok.o: ./../icmp/icerrok.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
icmp.o: ./../icmp/icmp.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
icmp_in.o: ./../icmp/icmp_in.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
icredirect.o: ./../icmp/icredirect.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
icsetbuf.o: ./../icmp/icsetbuf.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
icsetdata.o: ./../icmp/icsetdata.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
icsetsrc.o: ./../icmp/icsetsrc.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
hgadd.o: ./../igmp/hgadd.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/igmp.h
hgarpadd.o: ./../igmp/hgarpadd.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/igmp.h
hgarpdel.o: ./../igmp/hgarpdel.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/igmp.h
hginit.o: ./../igmp/hginit.c ../h/conf.h ../h/kernel.h ../h/sleep.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/igmp.h
hgjoin.o: ./../igmp/hgjoin.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/igmp.h
hgleave.o: ./../igmp/hgleave.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/igmp.h
hglookup.o: ./../igmp/hglookup.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/igmp.h
hgprint.o: ./../igmp/hgprint.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/igmp.h
hgrand.o: ./../igmp/hgrand.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/igmp.h
igmp.o: ./../igmp/igmp.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/igmp.h
igmp_in.o: ./../igmp/igmp_in.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/igmp.h
igmp_settimers.o: ./../igmp/igmp_settimers.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/igmp.h
igmp_update.o: ./../igmp/igmp_update.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/igmp.h
ip_in.o: ./../ip/ip_in.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
ipdbc.o: ./../ip/ipdbc.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
ipdoopts.o: ./../ip/ipdoopts.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ipdstopts.o: ./../ip/ipdstopts.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ipfadd.o: ./../ip/ipfadd.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ipfcons.o: ./../ip/ipfcons.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
ipfhcopy.o: ./../ip/ipfhcopy.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ipfinit.o: ./../ip/ipfinit.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
ipfjoin.o: ./../ip/ipfjoin.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ipfsend.o: ./../ip/ipfsend.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
ipftimer.o: ./../ip/ipftimer.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ipgetp.o: ./../ip/ipgetp.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
iph2net.o: ./../ip/iph2net.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
ipnet2h.o: ./../ip/ipnet2h.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
ipproc.o: ./../ip/ipproc.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
ipputp.o: ./../ip/ipputp.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
ipreass.o: ./../ip/ipreass.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/q.h
ipredirect.o: ./../ip/ipredirect.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ipsend.o: ./../ip/ipsend.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
isbrc.o: ./../ip/isbrc.c ../h/conf.h ../h/kernel.h ../h/sleep.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
rtadd.o: ./../ip/rtadd.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
rtdel.o: ./../ip/rtdel.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
rtdump.o: ./../ip/rtdump.c ../h/conf.h ../h/kernel.h ../h/sleep.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
rtfree.o: ./../ip/rtfree.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
rtget.o: ./../ip/rtget.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
rthash.o: ./../ip/rthash.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
rtinit.o: ./../ip/rtinit.c ../h/conf.h ../h/kernel.h ../h/sleep.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
rtnew.o: ./../ip/rtnew.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
rttimer.o: ./../ip/rttimer.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
bootp.o: ./../net/bootp.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/proc.h ../h/bootp.h
dnparse.o: ./../net/dnparse.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/ctype.h
dot2ip.o: ./../net/dot2ip.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
ethmatch.o: ./../net/ethmatch.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ethmcast.o: ./../net/ethmcast.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
getaddr.o: ./../net/getaddr.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
getconfig.o: ./../net/getconfig.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
getname.o: ./../net/getname.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
getnet.o: ./../net/getnet.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
getutim.o: ./../net/getutim.c ../h/conf.h ../h/kernel.h ../h/sleep.h \
 ../h/date.h ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h \
 ../h/ipreass.h ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h \
 ../h/tcpstat.h ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h \
 ../h/fserver.h ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h \
 ../h/route.h ../h/daemon.h ../h/snmpvars.h ../h/bootp.h
initgate.o: ./../net/initgate.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/bootp.h
inithost.o: ./../net/inithost.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ip2dot.o: ./../net/ip2dot.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
ip2name.o: ./../net/ip2name.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/bootp.h
local_out.o: ./../net/local_out.c ../h/conf.h ../h/kernel.h \
 ../h/proc.h ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h \
 ../h/ipreass.h ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h \
 ../h/tcpstat.h ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h \
 ../h/fserver.h ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h \
 ../h/route.h ../h/daemon.h ../h/snmpvars.h
name2ip.o: ./../net/name2ip.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/bootp.h
netdump.o: ./../net/netdump.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
netmask.o: ./../net/netmask.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
netmatch.o: ./../net/netmatch.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
netnum.o: ./../net/netnum.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
netstart.o: ./../net/netstart.c ../h/conf.h ../h/kernel.h ../h/sleep.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/snmp.h ../h/tcptimer.h \
 ../h/bootp.h ../h/q.h
netwrite.o: ./../net/netwrite.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/q.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ni_in.o: ./../net/ni_in.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h ../h/ospf_pkt.h \
 ../h/ospf_if.h ../h/ospf_db.h
othinit.o: ./../net/othinit.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/q.h
othwrite.o: ./../net/othwrite.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
rarp_in.o: ./../net/rarp_in.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
rarpsend.o: ./../net/rarpsend.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
setmask.o: ./../net/setmask.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
slowtimer.o: ./../net/slowtimer.c ../h/conf.h ../h/kernel.h \
 ../h/proc.h ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h \
 ../h/ipreass.h ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h \
 ../h/tcpstat.h ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h \
 ../h/fserver.h ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h \
 ../h/route.h ../h/daemon.h ../h/snmpvars.h
tftp.o: ./../net/tftp.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/tftp.h
nm_in.o: ./../netmon/nm_in.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
nm_init.o: ./../netmon/nm_init.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/netmon.h
stlookup.o: ./../netmon/stlookup.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
stnew.o: ./../netmon/stnew.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/netmon.h
echod.o: ./../netapp/echod.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
egp.o: ./../netapp/egp.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
fingerd.o: ./../netapp/fingerd.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/syslog.h
lfing.o: ./../netapp/lfing.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/tty.h ../h/user.h
ripadd.o: ./../netapp/ripadd.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ripcheck.o: ./../netapp/ripcheck.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ripifset.o: ./../netapp/ripifset.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ripin.o: ./../netapp/ripin.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/proc.h
ripmetric.o: ./../netapp/ripmetric.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ripok.o: ./../netapp/ripok.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
ripout.o: ./../netapp/ripout.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
riprecv.o: ./../netapp/riprecv.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
riprepl.o: ./../netapp/riprepl.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ripsend.o: ./../netapp/ripsend.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ripstart.o: ./../netapp/ripstart.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
rwho.o: ./../netapp/rwho.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/rwho.h
rwhod.o: ./../netapp/rwhod.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/rwho.h ../h/shell.h ../h/mark.h
rwhoind.o: ./../netapp/rwhoind.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/rwho.h
snmpd.o: ./../netapp/snmpd.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
udpecho.o: ./../netapp/udpecho.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
mount.o: ./../rfs/mount.c ../h/conf.h ../h/kernel.h ../h/name.h
rfalloc.o: ./../rfs/rfalloc.c ../h/conf.h ../h/kernel.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h
rfclose.o: ./../rfs/rfclose.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
rfcntl.o: ./../rfs/rfcntl.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
rfdump.o: ./../rfs/rfdump.c ../h/conf.h ../h/kernel.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h
rfgetc.o: ./../rfs/rfgetc.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
rfinit.o: ./../rfs/rfinit.c ../h/conf.h ../h/kernel.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h
rfio.o: ./../rfs/rfio.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
rfmkpac.o: ./../rfs/rfmkpac.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
rfopen.o: ./../rfs/rfopen.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
rfputc.o: ./../rfs/rfputc.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
rfread.o: ./../rfs/rfread.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
rfseek.o: ./../rfs/rfseek.c ../h/conf.h ../h/kernel.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h
rfsend.o: ./../rfs/rfsend.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/bootp.h
rfwrite.o: ./../rfs/rfwrite.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
unmount.o: ./../rfs/unmount.c ../h/conf.h ../h/kernel.h ../h/name.h
a1rwint.o: ./../snmp/a1rwint.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
a1rwlen.o: ./../snmp/a1rwlen.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
a1rwoid.o: ./../snmp/a1rwoid.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
a1rwval.o: ./../snmp/a1rwval.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
mksnmp.o: ./../snmp/mksnmp.c ../h/conf.h ../h/kernel.h ../h/sleep.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sae_findn.o: ./../snmp/sae_findn.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sae_get.o: ./../snmp/sae_get.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sae_getf.o: ./../snmp/sae_getf.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sae_getn.o: ./../snmp/sae_getn.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sae_match.o: ./../snmp/sae_match.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sae_set.o: ./../snmp/sae_set.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sat_findn.o: ./../snmp/sat_findn.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sat_get.o: ./../snmp/sat_get.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sat_getf.o: ./../snmp/sat_getf.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sat_getn.o: ./../snmp/sat_getn.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sat_match.o: ./../snmp/sat_match.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sat_set.o: ./../snmp/sat_set.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sif_get.o: ./../snmp/sif_get.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sif_getf.o: ./../snmp/sif_getf.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sif_getn.o: ./../snmp/sif_getn.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sif_match.o: ./../snmp/sif_match.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sif_set.o: ./../snmp/sif_set.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
sna2b.o: ./../snmp/sna2b.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
snb2a.o: ./../snmp/snb2a.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
snclient.o: ./../snmp/snclient.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
snerr.o: ./../snmp/snerr.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
snfreebl.o: ./../snmp/snfreebl.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
snhash.o: ./../snmp/snhash.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
sninit.o: ./../snmp/sninit.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
snleaf.o: ./../snmp/snleaf.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
snmib.o: ./../snmp/snmib.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
snoip.o: ./../snmp/snoip.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
snparse.o: ./../snmp/snparse.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
snprint.o: ./../snmp/snprint.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
snrslv.o: ./../snmp/snrslv.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
sntable.o: ./../snmp/sntable.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
srt_findn.o: ./../snmp/srt_findn.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
srt_get.o: ./../snmp/srt_get.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
srt_getf.o: ./../snmp/srt_getf.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
srt_getn.o: ./../snmp/srt_getn.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
srt_match.o: ./../snmp/srt_match.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
srt_set.o: ./../snmp/srt_set.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
stc_findn.o: ./../snmp/stc_findn.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
stc_get.o: ./../snmp/stc_get.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
stc_getf.o: ./../snmp/stc_getf.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
stc_getn.o: ./../snmp/stc_getn.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
stc_match.o: ./../snmp/stc_match.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
stc_set.o: ./../snmp/stc_set.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
blkcmp.o: ./../sys/blkcmp.c
blkcopy.o: ./../sys/blkcopy.c ../h/kernel.h
blkequ.o: ./../sys/blkequ.c ../h/kernel.h
chprio.o: ./../sys/chprio.c ../h/conf.h ../h/kernel.h ../h/proc.h
ckmode.o: ./../sys/ckmode.c ../h/conf.h ../h/kernel.h ../h/file.h
clkinit.o: ./../sys/clkinit.c ../h/conf.h ../h/kernel.h ../h/sleep.h \
 ../h/i386.h
close.o: ./../sys/close.c ../h/conf.h ../h/kernel.h ../h/io.h
conf.o: ./../sys/conf.c ../h/conf.h
control.o: ./../sys/control.c ../h/conf.h ../h/kernel.h ../h/io.h
create.o: ./../sys/create.c ../h/conf.h ../h/i386.h ../h/kernel.h \
 ../h/proc.h ../h/sem.h ../h/mem.h ../h/io.h
dotrace.o: ./../sys/dotrace.c ../h/kernel.h
evec.o: ./../sys/evec.c ../h/conf.h ../h/i386.h ../h/kernel.h \
 ../h/proc.h ../h/sem.h ../h/mem.h ../h/tty.h ../h/q.h ../h/io.h
freebuf.o: ./../sys/freebuf.c ../h/conf.h ../h/kernel.h ../h/mark.h \
 ../h/bufpool.h
freemem.o: ./../sys/freemem.c ../h/conf.h ../h/kernel.h ../h/mem.h
getbuf.o: ./../sys/getbuf.c ../h/conf.h ../h/kernel.h ../h/mark.h \
 ../h/bufpool.h
getc.o: ./../sys/getc.c ../h/conf.h ../h/kernel.h ../h/io.h
getitem.o: ./../sys/getitem.c ../h/conf.h ../h/kernel.h ../h/q.h
getmem.o: ./../sys/getmem.c ../h/conf.h ../h/kernel.h ../h/mem.h
getpid.o: ./../sys/getpid.c ../h/conf.h ../h/kernel.h ../h/proc.h
getprio.o: ./../sys/getprio.c ../h/conf.h ../h/kernel.h ../h/proc.h
getstk.o: ./../sys/getstk.c ../h/conf.h ../h/kernel.h ../h/mem.h
gettime.o: ./../sys/gettime.c ../h/conf.h ../h/kernel.h ../h/date.h
getuid.o: ./../sys/getuid.c ../h/conf.h ../h/kernel.h ../h/user.h
getuser.o: ./../sys/getuser.c ../h/conf.h ../h/kernel.h ../h/user.h
gpq.o: ./../sys/gpq.c ../h/conf.h ../h/kernel.h ../h/q.h
i386.o: ./../sys/i386.c ../h/icu.h ../h/i386.h ../h/kernel.h
init.o: ./../sys/init.c ../h/conf.h ../h/kernel.h ../h/io.h
initialize.o: ./../sys/initialize.c ../h/conf.h ../h/i386.h \
 ../h/kernel.h ../h/proc.h ../h/sem.h ../h/sleep.h ../h/mem.h \
 ../h/tty.h ../h/q.h ../h/io.h ../h/network.h ../h/systypes.h \
 ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h ../h/udp.h \
 ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h ../h/net.h \
 ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h ../h/file.h \
 ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h ../h/snmpvars.h
insert.o: ./../sys/insert.c ../h/conf.h ../h/kernel.h ../h/q.h
insertd.o: ./../sys/insertd.c ../h/conf.h ../h/kernel.h ../h/q.h
ioerr.o: ./../sys/ioerr.c ../h/conf.h ../h/kernel.h
ionull.o: ./../sys/ionull.c ../h/conf.h ../h/kernel.h
kill.o: ./../sys/kill.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/sem.h ../h/mem.h ../h/io.h
kprintf.o: ./../sys/kprintf.c ../h/conf.h ../h/kernel.h ../h/tty.h
kputc.o: ./../sys/kputc.c ../h/conf.h ../h/kernel.h ../h/tty.h
main.o: ./../sys/main.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/user.h ../h/proc.h
mark.o: ./../sys/mark.c ../h/conf.h ../h/kernel.h ../h/mark.h
mkpool.o: ./../sys/mkpool.c ../h/conf.h ../h/kernel.h ../h/mark.h \
 ../h/bufpool.h
naminit.o: ./../sys/naminit.c ../h/conf.h ../h/kernel.h ../h/name.h
nammap.o: ./../sys/nammap.c ../h/conf.h ../h/kernel.h ../h/name.h
namopen.o: ./../sys/namopen.c ../h/conf.h ../h/kernel.h ../h/name.h
namrepl.o: ./../sys/namrepl.c ../h/conf.h ../h/kernel.h ../h/name.h
newqueue.o: ./../sys/newqueue.c ../h/conf.h ../h/kernel.h ../h/q.h
open.o: ./../sys/open.c ../h/conf.h ../h/kernel.h ../h/io.h
panic.o: ./../sys/panic.c ../h/kernel.h ../h/proc.h
pcount.o: ./../sys/pcount.c ../h/conf.h ../h/kernel.h ../h/mark.h \
 ../h/ports.h
pcreate.o: ./../sys/pcreate.c ../h/conf.h ../h/kernel.h ../h/mark.h \
 ../h/ports.h
pdelete.o: ./../sys/pdelete.c ../h/conf.h ../h/kernel.h ../h/mark.h \
 ../h/ports.h
pinit.o: ./../sys/pinit.c ../h/conf.h ../h/kernel.h ../h/mark.h \
 ../h/ports.h
poolinit.o: ./../sys/poolinit.c ../h/conf.h ../h/kernel.h ../h/mark.h \
 ../h/bufpool.h
preceive.o: ./../sys/preceive.c ../h/conf.h ../h/kernel.h ../h/mark.h \
 ../h/ports.h
preset.o: ./../sys/preset.c ../h/conf.h ../h/kernel.h ../h/mark.h \
 ../h/ports.h
printq.o: ./../sys/printq.c ../h/q.h
psend.o: ./../sys/psend.c ../h/conf.h ../h/kernel.h ../h/mark.h \
 ../h/ports.h
ptclear.o: ./../sys/ptclear.c ../h/conf.h ../h/kernel.h ../h/mark.h \
 ../h/ports.h
putc.o: ./../sys/putc.c ../h/conf.h ../h/kernel.h ../h/io.h
queue.o: ./../sys/queue.c ../h/conf.h ../h/kernel.h ../h/q.h
read.o: ./../sys/read.c ../h/conf.h ../h/kernel.h ../h/io.h
ready.o: ./../sys/ready.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/q.h
receive.o: ./../sys/receive.c ../h/conf.h ../h/kernel.h ../h/proc.h
recvclr.o: ./../sys/recvclr.c ../h/conf.h ../h/kernel.h ../h/proc.h
recvtim.o: ./../sys/recvtim.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/q.h ../h/sleep.h
remove.o: ./../sys/remove.c ../h/conf.h ../h/kernel.h ../h/file.h \
 ../h/name.h
rename.o: ./../sys/rename.c ../h/conf.h ../h/kernel.h ../h/file.h \
 ../h/name.h
resched.o: ./../sys/resched.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/q.h
resume.o: ./../sys/resume.c ../h/conf.h ../h/kernel.h ../h/proc.h
scount.o: ./../sys/scount.c ../h/conf.h ../h/kernel.h ../h/sem.h
screate.o: ./../sys/screate.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/q.h ../h/sem.h
sdelete.o: ./../sys/sdelete.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/q.h ../h/sem.h
seek.o: ./../sys/seek.c ../h/conf.h ../h/kernel.h ../h/io.h
send.o: ./../sys/send.c ../h/conf.h ../h/kernel.h ../h/proc.h
sendf.o: ./../sys/sendf.c ../h/conf.h ../h/kernel.h ../h/proc.h
setdev.o: ./../sys/setdev.c ../h/conf.h ../h/kernel.h ../h/proc.h
setnok.o: ./../sys/setnok.c ../h/conf.h ../h/kernel.h ../h/proc.h
signal.o: ./../sys/signal.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/q.h ../h/sem.h
signaln.o: ./../sys/signaln.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/q.h ../h/sem.h
sleep.o: ./../sys/sleep.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/q.h ../h/sleep.h
sleep10.o: ./../sys/sleep10.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/q.h ../h/sleep.h
sleep100.o: ./../sys/sleep100.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/q.h ../h/sleep.h
sreset.o: ./../sys/sreset.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/q.h ../h/sem.h
ssclock.o: ./../sys/ssclock.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/q.h ../h/sleep.h
stacktrace.o: ./../sys/stacktrace.c ../h/conf.h ../h/kernel.h \
 ../h/proc.h
suspend.o: ./../sys/suspend.c ../h/conf.h ../h/kernel.h ../h/proc.h
syslog.o: ./../sys/syslog.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/io.h ../h/syslog.h ../h/bootp.h
unsleep.o: ./../sys/unsleep.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/q.h ../h/sleep.h
userret.o: ./../sys/userret.c ../h/conf.h ../h/kernel.h
wait.o: ./../sys/wait.c ../h/conf.h ../h/kernel.h ../h/proc.h ../h/q.h \
 ../h/sem.h
wakeup.o: ./../sys/wakeup.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/q.h ../h/sleep.h
write.o: ./../sys/write.c ../h/conf.h ../h/kernel.h ../h/io.h
xdone.o: ./../sys/xdone.c ../h/kernel.h
tcp_in.o: ./../tcp/tcp_in.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpabort.o: ./../tcp/tcpabort.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpacked.o: ./../tcp/tcpacked.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpackit.o: ./../tcp/tcpackit.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpcksum.o: ./../tcp/tcpcksum.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpclosed.o: ./../tcp/tcpclosed.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpclosewait.o: ./../tcp/tcpclosewait.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpclosing.o: ./../tcp/tcpclosing.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpdata.o: ./../tcp/tcpdata.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpdemux.o: ./../tcp/tcpdemux.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpdodat.o: ./../tcp/tcpdodat.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpestablished.o: ./../tcp/tcpestablished.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpfin1.o: ./../tcp/tcpfin1.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpfin2.o: ./../tcp/tcpfin2.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcph2net.o: ./../tcp/tcph2net.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcphowmuch.o: ./../tcp/tcphowmuch.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpidle.o: ./../tcp/tcpidle.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpinp.o: ./../tcp/tcpinp.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpiss.o: ./../tcp/tcpiss.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpkick.o: ./../tcp/tcpkick.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpkilltimers.o: ./../tcp/tcpkilltimers.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcplastack.o: ./../tcp/tcplastack.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcplisten.o: ./../tcp/tcplisten.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpnet2h.o: ./../tcp/tcpnet2h.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpok.o: ./../tcp/tcpok.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpopts.o: ./../tcp/tcpopts.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpostate.o: ./../tcp/tcpostate.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpout.o: ./../tcp/tcpout.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcppersist.o: ./../tcp/tcppersist.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpreset.o: ./../tcp/tcpreset.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcprexmt.o: ./../tcp/tcprexmt.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcprmss.o: ./../tcp/tcprmss.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcprtt.o: ./../tcp/tcprtt.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcprwindow.o: ./../tcp/tcprwindow.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpsend.o: ./../tcp/tcpsend.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpsmss.o: ./../tcp/tcpsmss.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpsndlen.o: ./../tcp/tcpsndlen.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/q.h
tcpswindow.o: ./../tcp/tcpswindow.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpswitch.o: ./../tcp/tcpswitch.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpsync.o: ./../tcp/tcpsync.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpsynrcvd.o: ./../tcp/tcpsynrcvd.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpsynsent.o: ./../tcp/tcpsynsent.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcptimer.o: ./../tcp/tcptimer.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/tcptimer.h
tcptimewait.o: ./../tcp/tcptimewait.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpwait.o: ./../tcp/tcpwait.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpwakeup.o: ./../tcp/tcpwakeup.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpwinit.o: ./../tcp/tcpwinit.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpxmit.o: ./../tcp/tcpxmit.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tfcoalesce.o: ./../tcp/tfcoalesce.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tfinsert.o: ./../tcp/tfinsert.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/q.h
tmclear.o: ./../tcp/tmclear.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/tcptimer.h
tmleft.o: ./../tcp/tmleft.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/tcptimer.h
tmset.o: ./../tcp/tmset.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/tcptimer.h
tqdump.o: ./../tcp/tqdump.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/tcptimer.h
tcballoc.o: ./../tcpd/tcballoc.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcbdealloc.o: ./../tcpd/tcbdealloc.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpbind.o: ./../tcpd/tcpbind.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpclose.o: ./../tcpd/tcpclose.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpcntl.o: ./../tcpd/tcpcntl.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpcon.o: ./../tcpd/tcpcon.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpgetc.o: ./../tcpd/tcpgetc.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpgetdata.o: ./../tcpd/tcpgetdata.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpgetspace.o: ./../tcpd/tcpgetspace.c ../h/conf.h ../h/kernel.h \
 ../h/proc.h ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h \
 ../h/ipreass.h ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h \
 ../h/tcpstat.h ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h \
 ../h/fserver.h ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h \
 ../h/route.h ../h/daemon.h ../h/snmpvars.h
tcpinit.o: ./../tcpd/tcpinit.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/mark.h
tcplq.o: ./../tcpd/tcplq.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/mark.h ../h/ports.h
tcpmcntl.o: ./../tcpd/tcpmcntl.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpmopen.o: ./../tcpd/tcpmopen.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/proc.h
tcpnxtp.o: ./../tcpd/tcpnxtp.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpputc.o: ./../tcpd/tcpputc.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpread.o: ./../tcpd/tcpread.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpserver.o: ./../tcpd/tcpserver.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpstat.o: ./../tcpd/tcpstat.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpuopt.o: ./../tcpd/tcpuopt.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpwr.o: ./../tcpd/tcpwr.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
tcpwrite.o: ./../tcpd/tcpwrite.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ttyalloc.o: ./../tty/ttyalloc.c ../h/conf.h ../h/kernel.h ../h/tty.h
ttycntl.o: ./../tty/ttycntl.c ../h/conf.h ../h/kernel.h ../h/tty.h
ttygetc.o: ./../tty/ttygetc.c ../h/conf.h ../h/kernel.h ../h/tty.h
ttyiin.o: ./../tty/ttyiin.c ../h/conf.h ../h/kernel.h ../h/tty.h
ttyinit.o: ./../tty/ttyinit.c ../h/conf.h ../h/kernel.h ../h/tty.h
ttynew.o: ./../tty/ttynew.c ../h/conf.h ../h/kernel.h ../h/tty.h
ttyoin.o: ./../tty/ttyoin.c ../h/conf.h ../h/kernel.h ../h/tty.h
ttyopen.o: ./../tty/ttyopen.c ../h/conf.h ../h/kernel.h ../h/tty.h
ttyputc.o: ./../tty/ttyputc.c ../h/conf.h ../h/kernel.h ../h/tty.h
ttyread.o: ./../tty/ttyread.c ../h/conf.h ../h/kernel.h ../h/tty.h
ttywrite.o: ./../tty/ttywrite.c ../h/conf.h ../h/kernel.h ../h/tty.h
udp_in.o: ./../udp/udp_in.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
udpcksum.o: ./../udp/udpcksum.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
udph2net.o: ./../udp/udph2net.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
udpnet2h.o: ./../udp/udpnet2h.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
udpnxtp.o: ./../udp/udpnxtp.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
udpsend.o: ./../udp/udpsend.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/igmp.h
upalloc.o: ./../udp/upalloc.c ../h/conf.h ../h/kernel.h ../h/proc.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
db_init.o: ./../ospf/db_init.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
db_lookup.o: ./../ospf/db_lookup.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
db_new.o: ./../ospf/db_new.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h ../h/ospf_pkt.h \
 ../h/ospf_if.h ../h/ospf_db.h
db_nlink.o: ./../ospf/db_nlink.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
db_resync.o: ./../ospf/db_resync.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
db_rlink.o: ./../ospf/db_rlink.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
db_update.o: ./../ospf/db_update.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
dd_queue.o: ./../ospf/dd_queue.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
dd_xmit.o: ./../ospf/dd_xmit.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ddi_exchng.o: ./../ospf/ddi_exchng.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ddi_exstart.o: ./../ospf/ddi_exstart.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ddi_full.o: ./../ospf/ddi_full.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
fcksum.o: ./../ospf/fcksum.c
if_elect.o: ./../ospf/if_elect.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
if_elect1.o: ./../ospf/if_elect1.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
lsa_add.o: ./../ospf/lsa_add.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
lsa_send.o: ./../ospf/lsa_send.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
lsa_xmit.o: ./../ospf/lsa_xmit.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
lsack_in.o: ./../ospf/lsack_in.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
lsr_add.o: ./../ospf/lsr_add.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
lsr_check.o: ./../ospf/lsr_check.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
lsr_in.o: ./../ospf/lsr_in.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h ../h/ospf_pkt.h \
 ../h/ospf_if.h ../h/ospf_db.h
lsr_queue.o: ./../ospf/lsr_queue.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
lsr_xmit.o: ./../ospf/lsr_xmit.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
lss_build.o: ./../ospf/lss_build.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
lsu_in.o: ./../ospf/lsu_in.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h ../h/ospf_pkt.h \
 ../h/ospf_if.h ../h/ospf_db.h
nb_add.o: ./../ospf/nb_add.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h ../h/ospf_pkt.h \
 ../h/ospf_if.h ../h/ospf_db.h
nb_aok.o: ./../ospf/nb_aok.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h ../h/ospf_pkt.h \
 ../h/ospf_if.h ../h/ospf_db.h
nb_clearl.o: ./../ospf/nb_clearl.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
nb_makel.o: ./../ospf/nb_makel.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h ../h/q.h
nb_mismatch.o: ./../ospf/nb_mismatch.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
nb_reform.o: ./../ospf/nb_reform.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
nb_rexmt.o: ./../ospf/nb_rexmt.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
nb_switch.o: ./../ospf/nb_switch.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospf.o: ./../ospf/ospf.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h ../h/proc.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospf_ddin.o: ./../ospf/ospf_ddin.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospf_hin.o: ./../ospf/ospf_hin.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospf_hsend.o: ./../ospf/ospf_hsend.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospf_in.o: ./../ospf/ospf_in.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospfcheck.o: ./../ospf/ospfcheck.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospfddtmpl.o: ./../ospf/ospfddtmpl.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospfhello.o: ./../ospf/ospfhello.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospfhtmpl.o: ./../ospf/ospfhtmpl.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospfifinit.o: ./../ospf/ospfifinit.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospfinit.o: ./../ospf/ospfinit.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospflsrtmpl.o: ./../ospf/ospflsrtmpl.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospflstmpl.o: ./../ospf/ospflstmpl.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospfnet2h.o: ./../ospf/ospfnet2h.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
ospftimer.o: ./../ospf/ospftimer.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
arpdump.o: ./../debug/arpdump.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
dnsdump.o: ./../debug/dnsdump.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
hexdump.o: ./../debug/hexdump.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
icmpdump.o: ./../debug/icmpdump.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
igmpdump.o: ./../debug/igmpdump.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/igmp.h
ipdump.o: ./../debug/ipdump.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
ospfdump.o: ./../debug/ospfdump.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ospf.h ../h/ospf_ls.h \
 ../h/ospf_pkt.h ../h/ospf_if.h ../h/ospf_db.h
pdump.o: ./../debug/pdump.c ../h/conf.h ../h/kernel.h ../h/network.h \
 ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h ../h/icmp.h \
 ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h ../h/tcb.h \
 ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h ../h/rfile.h \
 ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h ../h/daemon.h \
 ../h/snmpvars.h
tcpdump.o: ./../debug/tcpdump.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
udpdump.o: ./../debug/udpdump.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h
ee_cmd.o: ./../ether/ee/ee_cmd.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/i386.h ../h/ee.h ../h/i82586.h
ee_cntl.o: ./../ether/ee/ee_cntl.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/i386.h ../h/ee.h ../h/i82586.h
ee_demux.o: ./../ether/ee/ee_demux.c ../h/conf.h ../h/kernel.h \
 ../h/proc.h ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h \
 ../h/ipreass.h ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h \
 ../h/tcpstat.h ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h \
 ../h/fserver.h ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h \
 ../h/route.h ../h/daemon.h ../h/snmpvars.h ../h/ee.h ../h/i82586.h
ee_init.o: ./../ether/ee/ee_init.c ../h/conf.h ../h/kernel.h \
 ../h/proc.h ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h \
 ../h/ipreass.h ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h \
 ../h/tcpstat.h ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h \
 ../h/fserver.h ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h \
 ../h/route.h ../h/daemon.h ../h/snmpvars.h ../h/i386.h ../h/ee.h \
 ../h/i82586.h ../h/q.h
ee_intr.o: ./../ether/ee/ee_intr.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ee.h ../h/i82586.h
ee_mcast.o: ./../ether/ee/ee_mcast.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/i386.h ../h/ee.h ../h/i82586.h
ee_read.o: ./../ether/ee/ee_read.c ../h/conf.h ../h/kernel.h \
 ../h/proc.h ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h \
 ../h/ipreass.h ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h \
 ../h/tcpstat.h ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h \
 ../h/fserver.h ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h \
 ../h/route.h ../h/daemon.h ../h/snmpvars.h ../h/q.h
ee_rom.o: ./../ether/ee/ee_rom.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/i386.h ../h/ee.h ../h/i82586.h
ee_write.o: ./../ether/ee/ee_write.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ee.h ../h/i82586.h
ee_wstrt.o: ./../ether/ee/ee_wstrt.c ../h/conf.h ../h/kernel.h \
 ../h/network.h ../h/systypes.h ../h/ip.h ../h/ether.h ../h/ipreass.h \
 ../h/icmp.h ../h/udp.h ../h/tcp.h ../h/tcpfsm.h ../h/tcpstat.h \
 ../h/tcb.h ../h/net.h ../h/dgram.h ../h/arp.h ../h/fserver.h \
 ../h/rfile.h ../h/file.h ../h/domain.h ../h/netif.h ../h/route.h \
 ../h/daemon.h ../h/snmpvars.h ../h/ee.h ../h/i82586.h
eecmd.o: ./../mon/eecmd.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h ../h/./mon/monee.h \
 ../h/./mon/moni82586.h
eedemux.o: ./../mon/eedemux.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h ../h/./mon/monee.h \
 ../h/./mon/moni82586.h
eeinit.o: ./../mon/eeinit.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h ../h/./mon/moni386.h \
 ../h/./mon/monee.h ../h/./mon/moni82586.h
eeintr.o: ./../mon/eeintr.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h ../h/./mon/monee.h \
 ../h/./mon/moni82586.h
eerom.o: ./../mon/eerom.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h ../h/./mon/moni386.h \
 ../h/./mon/monee.h ../h/./mon/moni82586.h
eewrite.o: ./../mon/eewrite.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h ../h/./mon/monee.h \
 ../h/./mon/moni82586.h
eewstrt.o: ./../mon/eewstrt.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h ../h/./mon/monee.h \
 ../h/./mon/moni82586.h
monarp.o: ./../mon/monarp.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h
monboot.o: ./../mon/monboot.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h
monbootp.o: ./../mon/monbootp.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h ../h/./mon/monbootp.h
mongpq.o: ./../mon/mongpq.c ../h/./mon/monconf.h
monip.o: ./../mon/monip.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h
monitor.o: ./../mon/monitor.c ../h/conf.h ../h/com.h \
 ../h/./mon/monnetwork.h ../h/./mon/monsystypes.h ../h/./mon/monconf.h \
 ../h/./mon/monip.h ../h/./mon/monether.h ../h/./mon/monudp.h \
 ../h/./mon/monarp.h ../h/./mon/monnetif.h ../h/./mon/monitor.h \
 ../h/./mon/moni386.h ../h/./mon/monee.h ../h/./mon/moni82586.h
monnet.o: ./../mon/monnet.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h
montftp.o: ./../mon/montftp.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h ../h/./mon/montftp.h \
 ../h/./mon/monbootp.h
montimer.o: ./../mon/montimer.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h
monudp.o: ./../mon/monudp.c ../h/./mon/monnetwork.h \
 ../h/./mon/monsystypes.h ../h/./mon/monconf.h ../h/./mon/monip.h \
 ../h/./mon/monether.h ../h/./mon/monudp.h ../h/./mon/monarp.h \
 ../h/./mon/monnetif.h ../h/./mon/monitor.h ../h/./mon/montftp.h \
 ../h/./mon/monbootp.h
