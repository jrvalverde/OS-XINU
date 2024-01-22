#
# Makefile
#

LIBNAME =	unixsyscall.a

#
# Make variables
#
ROOTDIR	=	../../..
BIN	=	${ROOTDIR}/bin
LIB	=	${ROOTDIR}/lib
INC	=	${ROOTDIR}/include
CC	=	/bin/cc
AS	=	/bin/as
RM	=	/bin/rm
AR	=	/bin/ar
CP	=	/bin/cp
RANLIB	=	/bin/ranlib

# Machine specific things we must define
INSTR_TYPE	=	RISC
ARCH		=	SPARC
MODEL		=	SPARC_ELC
CODE_NAME	=	sun4c


HDIR	=	${INC}
CDEFS	=	-D$(INSTR_TYPE) -D$(ARCH) -D$(MODEL) -D$(CODE_NAME)
CFLAGS	=	-g -c -I${HDIR} ${CDEFS}
SFLAGS	=	-I${HDIR}
ASCPPFLAGS =	-I${HDIR} -DASM ${CDEFS}


.SUFFIXES:.o .c .s
.c.o:
	${CC} ${CFLAGS} -o $@ $<
.s.o:
	/lib/cpp ${ASCPPFLAGS} $< > .tmp.s
	${CC} -c ${SFLAGS} -o $@ .tmp.s

CFILES=		syscalintf.c systrap.s unix2xinu.c io.c environ.c
SFILES=		muldiv.s
XOFILES=	syscalintf.o systrap.o unix2xinu.o io.o environ.o muldiv.o \

${LIBNAME}:	$(XOFILES)
		${RM} -f unixsyscall.a
		${AR} cr unixsyscall.a ${XOFILES}
		${RANLIB} unixsyscall.a

install:	${LIBNAME}
		${CP} unixsyscall.a ${LIB}/unixsyscall.a
		${RANLIB} ${LIB}/unixsyscall.a

clean:
		rm -f ${LIBNAME} *.o ,* .,* core

