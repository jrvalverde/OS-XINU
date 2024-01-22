#
# Makefile for Page Server
#

.SUFFIXES: .o .c
DIR	=	../../..
BINDIR	=	${DIR}/bin
CC	=	/bin/cc
CPP	=	/lib/cpp
LD	= 	/bin/ld
CPU	=	SUN
#CPU	=	VAX
CFLAGS	=	-c -g -I. -D${CPU}

.c.o:
	${CC} ${CFLAGS} $<

all:	psv.o unix.o
	${CC} psv.o unix.o -o psv

install: all
	cp psv ${BINDIR}/psv

clean:
	/bin/rm -f *.o psv 

psv.o: psv.h
unix.o: tp.h
unix.o: psv.h
