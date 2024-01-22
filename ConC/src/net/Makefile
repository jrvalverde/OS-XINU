#	{ConC-dir}/src/net/Makefile
#

.SUFFIXES: .s .c .o

DIR=src/net

INCLUDE= ../../include

CFLAGS=	-c -O -I${INCLUDE}

CPP_FLAGS= -E -I${INCLUDE}

OBJS=	getnetbyaddr.o getnetbyname.o getnetent.o getproto.o getprotoent.o \
	getprotoname.o getservbyname.o getservbyport.o getservent.o \
	rcmd.o res_comp.o res_debug.o res_init.o res_mkquery.o \
	res_send.o rexec.o ruserpass.o

CFILES=	getnetbyaddr.c getnetbyname.c getnetent.c getproto.c getprotoent.c \
	getprotoname.c getservbyname.c getservbyport.c getservent.c \
	rcmd.c res_comp.c res_debug.c res_init.c res_mkquery.c \
	res_send.c rexec.c ruserpass.c

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
getnetbyaddr.o: ${INCLUDE}/kernel.h
getnetbyaddr.o: ${INCLUDE}/netdb.h
getnetbyname.o: ${INCLUDE}/kernel.h
getnetbyname.o: ${INCLUDE}/netdb.h
getnetent.o: ${INCLUDE}/kernel.h
getnetent.o: ${INCLUDE}/stdio.h
getnetent.o: ${INCLUDE}/netdb.h
getproto.o: ${INCLUDE}/kernel.h
getproto.o: ${INCLUDE}/netdb.h
getprotoent.o: ${INCLUDE}/kernel.h
getprotoent.o: ${INCLUDE}/stdio.h
getprotoent.o: ${INCLUDE}/netdb.h
getprotoname.o: ${INCLUDE}/kernel.h
getprotoname.o: ${INCLUDE}/netdb.h
getservbyname.o: ${INCLUDE}/kernel.h
getservbyname.o: ${INCLUDE}/netdb.h
getservbyport.o: ${INCLUDE}/kernel.h
getservbyport.o: ${INCLUDE}/netdb.h
getservent.o: ${INCLUDE}/kernel.h
getservent.o: ${INCLUDE}/stdio.h
getservent.o: ${INCLUDE}/netdb.h
rcmd.o: ${INCLUDE}/kernel.h
rcmd.o: ${INCLUDE}/stdio.h
rcmd.o: ${INCLUDE}/netdb.h
rcmd.o: ${INCLUDE}/errno.h
res_comp.o: ${INCLUDE}/stdio.h
res_debug.o: ${INCLUDE}/kernel.h
res_debug.o: ${INCLUDE}/stdio.h
res_init.o: ${INCLUDE}/kernel.h
res_init.o: ${INCLUDE}/stdio.h
res_mkquery.o: ${INCLUDE}/kernel.h
res_mkquery.o: ${INCLUDE}/stdio.h
res_send.o: ${INCLUDE}/kernel.h
res_send.o: ${INCLUDE}/stdio.h
res_send.o: ${INCLUDE}/errno.h
rexec.o: ${INCLUDE}/stdio.h
rexec.o: ${INCLUDE}/netdb.h
rexec.o: ${INCLUDE}/errno.h
ruserpass.o: ${INCLUDE}/kernel.h
ruserpass.o: ${INCLUDE}/stdio.h
ruserpass.o: ${INCLUDE}/errno.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
