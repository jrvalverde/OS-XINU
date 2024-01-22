#
#  Make the Xinu version of the C run-time support library
#

DIR	=	/usr/Xinu
LIB	=	$(DIR)/lib
AR	=	/bin/ar
HDIR	=	$(DIR)/include
CC	=	/bin/cc
CFLAGS	=	-O -c -I$(HDIR)
RANLIB	=	/usr/bin/ranlib

.s.o:
		$(CC) $(CFLAGS) $<
.c.o:
		$(CC) $(CFLAGS) $<

SFILES	=	udiv.s urem.s

CFILES	=	abs.c atoi.c atol.c ctype_.c			\
		doprnt.c doscan.c				\
		fgets.c fprintf.c fputs.c			\
		gets.c						\
		index.c						\
		printf.c puts.c					\
		qsort.c						\
		rand.c rindex.c					\
		scanf.c sprintf.c				\
		strcat.c strcmp.c strcpy.c strlen.c		\
		strncat.c strncmp.c strncpy.c swab.c

OFILES	=	abs.o atoi.o atol.o ctype_.o			\
		doprnt.o doscan.o				\
		fgets.o fprintf.o fputs.o			\
		gets.o						\
		index.o						\
		printf.o puts.o					\
		qsort.o						\
		rand.o rindex.o					\
		scanf.o sprintf.o				\
		strcat.o strcmp.o strcpy.o strlen.o		\
		strncat.o strncmp.o strncpy.o udiv.o urem.o


all:		libvxc.a

install:	all
		cp libvxc.a $(LIB)/libvxc.a
		$(RANLIB) $(LIB)/libvxc.a

clean:
		rm -rf *.o core a.out libvxc.a .,* ,*

libvxc.a:	$(OFILES)
		rm -rf libvxc.a
		$(AR) cr libvxc.a $(OFILES)
		$(RANLIB) libvxc.a
