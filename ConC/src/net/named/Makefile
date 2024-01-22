#	{ConC-dir}/src/net/named/Makefile
#

.SUFFIXES: .s .c .o

DIR=src/net/named

INCLUDE= ../../../include

CFLAGS=	-c -O -I${INCLUDE}

CPP_FLAGS= -E -I${INCLUDE}

OBJS=	gethostnamadr.o sethostent.o

CFILES=	gethostnamadr.c sethostent.c

SFILES= 

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
gethostnamadr.o: ${INCLUDE}/kernel.h
gethostnamadr.o: ${INCLUDE}/netdb.h
gethostnamadr.o: ${INCLUDE}/stdio.h
gethostnamadr.o: ${INCLUDE}/errno.h
sethostent.o: ${INCLUDE}/kernel.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
