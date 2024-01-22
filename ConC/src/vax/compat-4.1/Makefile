#	{ConC-dir}/src/vax/compat-4.1/Makefile
#

.SUFFIXES: .c .o

DIR=src/vax/compat-4.1

INCLUDE= ../../../include

CFLAGS=	-c -O -I${INCLUDE}

OBJS=	reset.o

CFILES=	reset.c

SFILES=

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
reset.o: ${INCLUDE}/kernel.h
reset.o: ${INCLUDE}/errno.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
