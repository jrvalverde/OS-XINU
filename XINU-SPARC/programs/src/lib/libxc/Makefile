#
#  Make the Xinu version of the C run-time support library
#

DIR	=	../../..
CFLAGS	=	-g -c -I${DIR}/include
SFLAGS	=	-I../../sys/h
AR	=	/bin/ar
LIB	=	${DIR}/lib

.c.o:
		${CC} ${CFLAGS} -o $@ $<
.s.o:
		$(AS) -o $@ $(AFLAGS) $<

CFILES	=	abs.c atof.c atoi.c atol.c bzero.c	\
		ctype_.c ecvt.c				\
		index.c  qsort.c			\
		rand.c rindex.c 			\
		strcat.c strcmp.c strcpy.c strlen.c	\
		strncat.c strncmp.c strncpy.c swab.c	\
		bcopy.c					\
		bcmp.c					\
		dir.c					\
		exit.c					\
		malloc.c				\
		getenv.c				\
		perror.c				\
		stdio/doprnt.c				\
		stdio/doscan.c				\
		stdio/fdopen.c				\
		stdio/fgetc.c				\
		stdio/fgets.c				\
		stdio/filbuf.c				\
		stdio/findiop.c				\
		stdio/flsbuf.c				\
		stdio/fopen.c				\
		stdio/fprintf.c				\
		stdio/fputc.c				\
		stdio/fputs.c				\
		stdio/fread.c				\
		stdio/freopen.c				\
		stdio/fseek.c				\
		stdio/ftell.c				\
		stdio/fwrite.c				\
		stdio/gets.c				\
		stdio/printf.c				\
		stdio/puts.c				\
		stdio/rew.c				\
		stdio/scanf.c				\
		stdio/sprintf.c				\
		stdio/ungetc.c



SFILES	=	muldiv.s	

OFILES	=	abs.o 					\
		atof.o					\
		atoi.o					\
		atol.o					\
		bzero.o					\
		ctype_.o				\
		ecvt.o					\
		index.o					\
		qsort.o					\
		rand.o					\
		rindex.o				\
		strcat.o				\
		strcmp.o				\
		strcpy.o				\
		strlen.o				\
		strncat.o				\
		strncmp.o				\
		strncpy.o				\
		swab.o					\
		bcopy.o					\
		bcmp.o					\
		dir.o					\
		exit.o					\
		malloc.o				\
		getenv.o				\
		perror.o				\
		stdio/doprnt.o				\
		stdio/doscan.o				\
		stdio/fdopen.o				\
		stdio/fgetc.o				\
		stdio/fgets.o				\
		stdio/filbuf.o				\
		stdio/findiop.o				\
		stdio/flsbuf.o				\
		stdio/fopen.o				\
		stdio/fprintf.o				\
		stdio/fputc.o				\
		stdio/fputs.o				\
		stdio/fread.o				\
		stdio/freopen.o				\
		stdio/fseek.o				\
		stdio/ftell.o				\
		stdio/fwrite.o				\
		stdio/gets.o				\
		stdio/printf.o				\
		stdio/puts.o				\
		stdio/rew.o				\
		stdio/scanf.o				\
		stdio/sprintf.o				\
		stdio/ungetc.o				\
		muldiv.o				\
#		SUN-ODIR/umultiply.o			\
#		SUN-ODIR/multiply.o			\
#		SUN-ODIR/rem.o				\
#		SUN-ODIR/div.o				


all:		libxc.a

libxc.a:	${OFILES}
		rm -f libxc.a
		ar cr libxc.a `lorder $(OFILES) | tsort`
#		ar cr libxc.a $(OFILES)
		ranlib libxc.a

install:	libxc.a
		cp libxc.a $(LIB)/libxc.a
		ranlib $(LIB)/libxc.a
		
clean:
		rm -f ${OFILES}
