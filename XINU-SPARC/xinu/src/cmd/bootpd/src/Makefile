#
# bootpd   - BOOTP server daemon
#
# $Header: /usr/src/local/etc/bootpd/src/RCS/Makefile,v 1.10 90/07/16 15:43:10 trinkle Exp $
#
# Copyright 1990 - Cypress Network Operations Center, Purdue University
#
HDR =	../include/args.h ../include/bool.h ../include/bootp.h		\
	../include/bootp_conf.h ../include/hash.h ../include/htype.h	\
	../include/ifaces.h ../include/list.h ../include/port_descr.h	\
	../include/sizes.h ../include/tags.h
SRC =	atend.c copy_entry.c detach.c enter.c findtag.c freelist.c	\
	get_htype.c getifaces.c getmtime.c getport.c hash.c init.c 	\
	inithash.c insert.c lookup.c main.c makename.c  		\
	my_best_ipaddr.c opensockets.c print_en.c print_ip.c 		\
	process_1084_vendor.c process_args.c process_request.c 		\
	process_xinu_vendor.c quit.c readfile.c reload.c send_reply.c	\
	server.c sigsetup.c xmalloc.c xrealloc.c
OBJ =	atend.o copy_entry.o detach.o enter.o findtag.o freelist.o	\
	get_htype.o getifaces.o getmtime.o getport.o hash.o init.o	\
	inithash.o insert.o lookup.o main.o makename.o 			\
	my_best_ipaddr.o opensockets.o print_en.o print_ip.o		\
	process_1084_vendor.o process_args.o process_request.o		\
	process_xinu_vendor.o quit.o readfile.o reload.o send_reply.o	\
	server.o sigsetup.o xmalloc.o xrealloc.o

DESTDIR=
CONFDIR=/etc
ETCDIR=/usr/local/etc

DEP = ${SRC}
INCLUDES = -I../include
LIBDIRS  = 
LIBS	 = 
CFLAGS 	 = -g ${INCLUDES} -DBOOTPD_DIR='"${CONFDIR}"'

INSTALL= install -m 775 -o root -g staff

all: bootpd

bootpd:	${OBJ}
	sh newvers.sh
	${CC} ${CFLAGS} -o bootpd version.c ${OBJ} ${LIBDIRS} ${LIBS}

install: all
	${INSTALL} -c -s bootpd ${DESTDIR}${ETCDIR}/bootpd

clean:
	@rm -f core bootpd *.BAK *.CKP *~ .emacs* *.o version.c errs ERRS

listall:
	@pr -f ${SRC} ${HDR}
	@echo "Complete: `date`" >> list

list: ${SRC} ${HDR}
	@pr $?
	@echo "Update:   `date`" >> list

depend:
	maketd ${INCLUDES} ${DEP}

# DO NOT DELETE THIS LINE - maketd DEPENDS ON IT
# Dependencies generated at: Thu Jul 12 10:48:21 EST 1990

atend.o: ../include/list.h
atend.o: atend.c
copy_entry.o: ../include/bootp_conf.h
copy_entry.o: ../include/list.h
copy_entry.o: ../include/sizes.h
copy_entry.o: copy_entry.c
detach.o: detach.c
enter.o: ../include/bootp_conf.h
enter.o: ../include/hash.h
enter.o: ../include/list.h
enter.o: ../include/null.h
enter.o: ../include/sizes.h
enter.o: ../include/tags.h
enter.o: enter.c
findtag.o: ../include/tags.h
findtag.o: findtag.c
freelist.o: ../include/list.h
freelist.o: freelist.c
get_htype.o: ../include/htype.h
get_htype.o: get_htype.c
getifaces.o: ../include/ifaces.h
getifaces.o: ../include/list.h
getifaces.o: ../include/null.h
getifaces.o: getifaces.c
getmtime.o: getmtime.c
getport.o: ../include/null.h
getport.o: getport.c
hash.o: ../include/hash.h
hash.o: ../include/list.h
hash.o: hash.c
init.o: init.c
inithash.o: ../include/hash.h
inithash.o: ../include/list.h
inithash.o: ../include/null.h
inithash.o: inithash.c
insert.o: ../include/hash.h
insert.o: ../include/list.h
insert.o: insert.c
lookup.o: ../include/hash.h
lookup.o: ../include/list.h
lookup.o: ../include/null.h
lookup.o: lookup.c
main.o: ../include/args.h
main.o: main.c
makename.o: ../include/bootp.h
makename.o: ../include/null.h
makename.o: makename.c
my_best_ipaddr.o: ../include/ifaces.h
my_best_ipaddr.o: ../include/list.h
my_best_ipaddr.o: ../include/sizes.h
my_best_ipaddr.o: my_best_ipaddr.c
opensockets.o: ../include/bootp.h
opensockets.o: ../include/null.h
opensockets.o: ../include/port_descr.h
opensockets.o: opensockets.c
print_en.o: print_en.c
print_ip.o: print_ip.c
process_1084_vendor.o: ../include/bootp.h
process_1084_vendor.o: ../include/bootp_conf.h
process_1084_vendor.o: ../include/list.h
process_1084_vendor.o: ../include/null.h
process_1084_vendor.o: ../include/sizes.h
process_1084_vendor.o: process_1084_vendor.c
process_args.o: ../include/args.h
process_args.o: process_args.c
process_request.o: ../include/bootp.h
process_request.o: ../include/bootp_conf.h
process_request.o: ../include/hash.h
process_request.o: ../include/list.h
process_request.o: ../include/null.h
process_request.o: ../include/sizes.h
process_request.o: process_request.c
process_xinu_vendor.o: ../include/bootp.h
process_xinu_vendor.o: ../include/bootp_conf.h
process_xinu_vendor.o: ../include/list.h
process_xinu_vendor.o: ../include/sizes.h
process_xinu_vendor.o: process_xinu_vendor.c
quit.o: ../include/port_descr.h
quit.o: quit.c
readfile.o: ../include/bool.h
readfile.o: ../include/bootp_conf.h
readfile.o: ../include/hash.h
readfile.o: ../include/list.h
readfile.o: ../include/sizes.h
readfile.o: readfile.c
reload.o: ../include/args.h
reload.o: ../include/bootp_conf.h
reload.o: ../include/hash.h
reload.o: ../include/list.h
reload.o: ../include/sizes.h
reload.o: reload.c
send_reply.o: ../include/bootp.h
send_reply.o: ../include/port_descr.h
send_reply.o: ../include/sizes.h
send_reply.o: send_reply.c
server.o: ../include/bool.h
server.o: ../include/bootp.h
server.o: ../include/bootp_conf.h
server.o: ../include/list.h
server.o: ../include/port_descr.h
server.o: ../include/sizes.h
server.o: server.c
sigsetup.o: sigsetup.c
xmalloc.o: xmalloc.c
xrealloc.o: xrealloc.c

# DO NOT ADD ANYTHING HERE - WILL GO AWAY
