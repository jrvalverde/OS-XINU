#
#  Make the run-time library for resident Ethernet downloader
#

AR	=	/bin/ar
DIR	=	/usr/Xinu
CC	=	$(DIR)/bin/cc11
CFLAGS	=	-c -O
LD	=	$(DIR)/bin/ld11
LORDER	=	/usr/bin/lorder
RANLIB	=	$(DIR)/bin/ranlib11

.s.o:
		$(CC) $(CFLAGS) $<

.c.o:
		$(CC) $(CFLAGS) $<

SFILES	=	aldiv.s almul.s alrem.s csv.s ldiv.s lmul.s lrem.s

CFILES	=	printf.c

OFILES	=	aldiv.o almul.o alrem.o csv.o ldiv.o lmul.o lrem.o printf.o

all:		libsa.a

install:	all
		cp libsa.a ../src
		$(RANLIB) ../src/libsa.a

clean:
		rm -rf *.o *.a a.out core

libsa.a:	$(OFILES)
		rm -rf libsa.a
		$(AR) cr libsa.a $(OFILES)
		$(RANLIB) libsa.a
