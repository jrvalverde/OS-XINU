#
# Makefile
#

PROGNAME =	ls

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
LD	=	/bin/ld -N -T 2000 -e unixstartup
CC	=	/bin/cc
AS	=	/bin/as
CFLAGS	=	-g -I${INC}
#
# user's own C lib (usually compiled from their local unix source code)
#		 	Alternative C lib
#
ALTCLIB	=	${LIB}/stdiolibu.a ${LIB}/genlibu.a ${LIB}/stdiolibu.a 



.c.o:
		${CC} -c ${CFLAGS} $<
.s.o:
		${AS} $<

CFILES=		${PROGNAME}.c 

XOFILES=	${PROGNAME}.o

${PROGNAME}:	$(XOFILES)
		${LD} $(XOFILES) ${CLIB} ${SLIB} -o ${PROGNAME}

alt:		$(XOFILES)
		${LD} $(XOFILES) ${ALTCLIB} ${SLIB} -o ${PROGNAME}

install:	${PROGNAME}
		cp ${PROGNAME} ${BIN}/${PROGNAME}

clean:
		rm -f ${PROGNAME} *.o ,* .,* core

