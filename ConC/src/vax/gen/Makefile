#	{ConC-dir}/src/vax/gen/Makefile
#

.SUFFIXES: .s .c .o

DIR=src/vax/gen

INCLUDE= ../../../include

CFLAGS=	-c -O -I${INCLUDE}

CPP_FLAGS= -E -I${INCLUDE}

OBJS=	_setjmp.o \
	abs.o alloca.o atof.o \
	bcmp.o bcopy.o bzero.o \
	ecvt.o \
	fabs.o ffs.o frexp.o \
	index.o insque.o \
	ldexp.o \
	modf.o \
	nargs.o \
	remque.o rindex.o \
	setjmp.o setjmperr.o strcat.o strcmp.o strcpy.o \
	strlen.o strncat.o strncmp.o strncpy.o \
	udiv.o urem.o
#	disktab.o #

CFILES=	_setjmp.c \
	ecvt.c setjmp.c setjmperr.c
#	disktab.c #

SFILES= _setjmp.s \
	abs.s alloca.s atof.s \
	bcmp.s bcopy.s bzero.s \
	fabs.s ffs.s frexp.s \
	index.s insque.s \
	ldexp.s \
	modf.s \
	nargs.s \
	remque.s rindex.s \
	setjmp.s strcat.s strcmp.s strcpy.s strlen.s \
	strncat.s strncmp.s strncpy.s \
	udiv.s urem.s

.s.o:
	/lib/cpp ${CPP_FLAGS} $*.s | ${AS} -o $*.o

.c.o:
	cc ${CFLAGS} $*.c

lib_flag:	${OBJS}
	touch lib_flag

clean:
	@echo "***  Cleaning ${DIR} ..."
	@/bin/rm -f ${OBJS} lib_flag

depend:
		@echo "***  Making ${DIR} depend  ..."
		@echo '^#include[ 	]*(<(DUMMY.x' > .ils
		@/bin/ls ${INCLUDE} >> .ils
		@echo 'DUMMY.x)>|")' >> .ils
		@(egrep -f .ils ${CFILES} ${SFILES} ${INCLUDE}/*.h	\
		| sed	-e 's/</"$${INCLUDE}\//;s/>/"/'			\
			-e 's/:[^"]*"\([^"]*\)".*/: \1/'		\
		| sed	-e 's/\.c/.o/'					\
			-e 's/\.s/.o/' >makedep)
		@echo '/^# DO NOT DELETE	THIS LINE/+2,$$d' >eddep
		@echo '$$r makedep' >>eddep
		@echo 'w' >>eddep
		@cp Makefile .,oldmake
		@cp Makefile ,Makefile
		@ed - ,Makefile < eddep
		@/bin/rm eddep makedep .ils
		@echo '#	DEPENDENCIES MUST END AT END OF	FILE' >>,Makefile
		@echo '#	IF YOU PUT STUFF HERE IT WILL GO AWAY'>>,Makefile
		@echo '#	see make depend	above'>>,Makefile
		@/bin/mv ,Makefile Makefile

# DO NOT DELETE	THIS LINE
# DEPENDENCIES MUST END	AT END OF FILE
_setjmp.o: ${INCLUDE}/kernel.h
_setjmp.o: ${INCLUDE}/errno.h
ecvt.o: ${INCLUDE}/kernel.h
setjmp.o: ${INCLUDE}/kernel.h
setjmp.o: ${INCLUDE}/errno.h
setjmperr.o: ${INCLUDE}/kernel.h
setjmperr.o: ${INCLUDE}/errno.h
abs.o: DEFS.h
alloca.o: DEFS.h
atof.o: DEFS.h
bcmp.o: DEFS.h
bcopy.o: DEFS.h
bzero.o: DEFS.h
fabs.o: DEFS.h
ffs.o: DEFS.h
frexp.o: DEFS.h
index.o: DEFS.h
insque.o: DEFS.h
ldexp.o: DEFS.h
ldexp.o: ${INCLUDE}/errno.h
modf.o: DEFS.h
nargs.o: DEFS.h
remque.o: DEFS.h
rindex.o: DEFS.h
strcat.o: DEFS.h
strcmp.o: DEFS.h
strcpy.o: DEFS.h
strlen.o: DEFS.h
strncat.o: DEFS.h
strncmp.o: DEFS.h
strncpy.o: DEFS.h
udiv.o: DEFS.h
udiv.o: DEFS.h
urem.o: DEFS.h
urem.o: DEFS.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
