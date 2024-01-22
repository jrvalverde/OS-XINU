#	{ConC-dir}/example/Makefile
#

.SUFFIXES: .c .a

CCC=	../bin/ccc

INCLUDE= ../include

CFLAGS=

OBJS=	ex1.a ex2.a ex3.a ex4.a ex5.a

CFILES=	ex1.c ex2.c ex3.c ex4.c ex5.c

SFILES=

.c.a:
	${CCC} ${CFLAGS} $*.c -o $*.a

all:	${OBJS}

clean:
	/bin/rm -f ${OBJS}

depend:
		@echo "***  Making example depend  ..."
		@echo '^#include[ 	]*(<(DUMMY.x' > .ils
		@/bin/ls ${INCLUDE} >> .ils
		@echo 'DUMMY.x)>|")' >> .ils
		@(egrep -f .ils ${CFILES} ${SFILES} ${INCLUDE}/*.h	\
		| sed	-e 's/</"$${INCLUDE}\//;s/>/"/'			\
			-e 's/:[^"]*"\([^"]*\)".*/: \1/'		\
		| sed	-e 's/\.c/.a/'	 				\
			-e 's/\.s/.a/' >makedep)
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
ex3.a: ${INCLUDE}/kernel.h
ex5.a: ${INCLUDE}/stdio.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
