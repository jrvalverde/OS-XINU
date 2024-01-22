#	{ConC-dir}/src/stdio/Makefile
#

.SUFFIXES: .s .c .o

DIR=src/stdio

INCLUDE= ../../include

CFLAGS=	-c -O -I${INCLUDE}

CPP_FLAGS= -E -I${INCLUDE}

OBJS=	clrerr.o \
	doscan.o \
	exit.o \
	fdopen.o feof.o ferror.o fgetc.o fgets.o filbuf.o fileno.o \
	findiop.o flsbuf.o fopen.o fprintf.o fputc.o fputs.o \
	fread.o freopen.o fseek.o ftell.o fwrite.o \
	getc.o getchar.o gets.o getw.o \
	printf.o putc.o putchar.o puts.o putw.o \
	rew.o \
	scanf.o setbuf.o setbuffer.o sibuf.o sobuf.o \
	sprintf.o stdio_.o strout.o \
	ungetc.o

CFILES=	clrerr.c \
	doscan.c \
	exit.c \
	fdopen.c feof.c ferror.c fgetc.c fgets.c filbuf.c fileno.c \
	findiop.c flsbuf.c fopen.c fprintf.c fputc.c fputs.c \
	fread.c freopen.c fseek.c ftell.c fwrite.c \
	getc.c getchar.c gets.c getw.c \
	printf.c putc.c putchar.c puts.c putw.c \
	rew.c \
	scanf.c setbuf.c setbuffer.c sibuf.c sobuf.c \
	sprintf.c stdio_.c strout.c \
	ungetc.c

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
clrerr.o: ${INCLUDE}/stdio.h
doscan.o: ${INCLUDE}/stdio.h
exit.o: ${INCLUDE}/kernel.h
exit.o: ${INCLUDE}/syscall.h
exit.o: ${INCLUDE}/proc.h
exit.o: ${INCLUDE}/stdio.h
exit.o: ${INCLUDE}/sem.h
fdopen.o: ${INCLUDE}/stdio.h
feof.o: ${INCLUDE}/stdio.h
ferror.o: ${INCLUDE}/stdio.h
fgetc.o: ${INCLUDE}/stdio.h
fgets.o: ${INCLUDE}/stdio.h
filbuf.o: ${INCLUDE}/stdio.h
fileno.o: ${INCLUDE}/stdio.h
findiop.o: ${INCLUDE}/kernel.h
findiop.o: ${INCLUDE}/stdio.h
findiop.o: ${INCLUDE}/errno.h
flsbuf.o: ${INCLUDE}/stdio.h
fopen.o: ${INCLUDE}/stdio.h
fopen.o: ${INCLUDE}/errno.h
fprintf.o: ${INCLUDE}/stdio.h
fputc.o: ${INCLUDE}/stdio.h
fputs.o: ${INCLUDE}/stdio.h
fread.o: ${INCLUDE}/stdio.h
freopen.o: ${INCLUDE}/stdio.h
freopen.o: ${INCLUDE}/errno.h
fseek.o: ${INCLUDE}/stdio.h
ftell.o: ${INCLUDE}/stdio.h
fwrite.o: ${INCLUDE}/stdio.h
getc.o: ${INCLUDE}/stdio.h
getchar.o: ${INCLUDE}/stdio.h
gets.o: ${INCLUDE}/stdio.h
getw.o: ${INCLUDE}/stdio.h
printf.o: ${INCLUDE}/stdio.h
putc.o: ${INCLUDE}/stdio.h
putchar.o: ${INCLUDE}/stdio.h
puts.o: ${INCLUDE}/stdio.h
putw.o: ${INCLUDE}/stdio.h
rew.o: ${INCLUDE}/stdio.h
scanf.o: ${INCLUDE}/stdio.h
setbuf.o: ${INCLUDE}/stdio.h
setbuffer.o: ${INCLUDE}/stdio.h
sibuf.o: ${INCLUDE}/stdio.h
sobuf.o: ${INCLUDE}/stdio.h
sprintf.o: ${INCLUDE}/stdio.h
stdio_.o: ${INCLUDE}/stdio.h
strout.o: ${INCLUDE}/stdio.h
ungetc.o: ${INCLUDE}/stdio.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
