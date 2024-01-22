#
# Makefile
#

.SUFFIXES:.o .c .s

#
# Make variables
#
ROOTDIR	=	../../..
BIN	=	${ROOTDIR}/bin
LIB	=	${ROOTDIR}/lib
INC	=	${ROOTDIR}/include
SLIB	=	${LIB}/unixsyscall.a
CLIB	=	${LIB}/libxc.a
LD	=	/bin/ld -T 2020 -e _unixstartup
CC	=	/bin/cc
AS	=	/bin/as
CFLAGS	=	-g -I${INC}
MV	=	/bin/mv
CP	=	/bin/cp
RM	=	/bin/rm
#
# user's own C lib (usually compiled from their local unix source code)
#		 	Alternative C lib
#
ALTCLIB	=	${LIB}/stdiolibu.a ${LIB}/genlibu.a ${LIB}/stdiolibu.a 

BINARIES =	cat cp close date echo kill mount mv rm unmount

.c.o:
		${CC} -c ${CFLAGS} $<
.s.o:
		${AS} $<

all:	cat cp close date echo kill mount mv rm unmount
altall:	altcat altcp altclose altdate altecho altkill altmount altmv altrm altunmount

cat:	cat.o
		${LD} cat.o  ${CLIB} ${SLIB} -o cat
altcat:		cat.o
		${LD} cat.o ${ALTCLIB} ${SLIB} -o cat

cp:	cp.o
		${LD} cp.o ${CLIB} ${SLIB} -o cp
altcp:	cp.o
		${LD} cp.o ${ALTCLIB} ${SLIB} -o cp

close:	close.o
		${LD} close.o ${CLIB} ${SLIB} -o close
altclose:	close.o
		${LD} close.o ${ALTCLIB} ${SLIB} -o close

date:	date.o
		${LD} date.o ${CLIB} ${SLIB} -o date
altdate:	date.o
		${LD} date.o ${ALTCLIB} ${SLIB} -o date

echo:	echo.o
		${LD} echo.o ${CLIB} ${SLIB} -o echo
altecho:	echo.o
		${LD} echo.o ${ALTCLIB} ${SLIB} -o echo

kill:	kill.o
		${LD} kill.o ${CLIB} ${SLIB} -o kill
altkill:	kill.o
		${LD} kill.o ${ALTCLIB} ${SLIB} -o kill

mount:	mount.o
		${LD} mount.o ${CLIB} ${SLIB} -o mount
altmount:	mount.o
		${LD} mount.o ${ALTCLIB} ${SLIB} -o mount

mv:	mv.o
		${LD} mv.o ${CLIB} ${SLIB} -o mv
altmv:	mv.o
		${LD} mv.o ${ALTCLIB} ${SLIB} -o mv

rm:	rm.o
		${LD} rm.o ${CLIB} ${SLIB} -o rm
altrm:	rm.o
		${LD} rm.o ${ALTCLIB} ${SLIB} -o rm

unmount:	unmount.o
		${LD} unmount.o ${CLIB} ${SLIB} -o unmount
altunmount:	unmount.o
		${LD} unmount.o ${ALTCLIB} ${SLIB} -o unmount

install:	${BIN}/cat ${BIN}/cp ${BIN}/close ${BIN}/date ${BIN}/echo ${BIN}/kill ${BIN}/mount ${BIN}/mv ${BIN}/rm ${BIN}/unmount

${BIN}/cat:	cat
		install cat ${BIN}/cat
${BIN}/cp:	cp
		install cp ${BIN}/cp
${BIN}/close:	close
		install close ${BIN}/close
${BIN}/date:	date
		install date ${BIN}/date
${BIN}/echo:	echo
		install echo ${BIN}/echo
${BIN}/kill:	kill
		install kill ${BIN}/kill
${BIN}/mount:	mount
		install mount ${BIN}/mount
${BIN}/mv:	mv
		install mv ${BIN}/mv
${BIN}/rm:	rm
		install rm ${BIN}/rm
${BIN}/unmount:	unmount
		install unmount ${BIN}/unmount


clean:
		-${RM} -rf ${BINARIES} *.o ,* .,* core make.out *.a

