#
#  make the 11 Cross-assembler that runs on the VAX
#

CFLAGS  = -O
LIBDIR  = /usr/Xinu/lib
DESTDIR = /usr/Xinu/bin

PASS1=as11.o as12.o as13.o as14.o as15.o as16.o as17.o as18.o as19.o

PASS1SRC=as11.c as12.c as13.c as14.c as15.c as16.c as17.c as18.c as19.c

PASS2=as21.o as22.o as23.o as24.o as25.o as26.o as27.o as28.o as29.o

PASS2SRC=as21.c as22.c as23.c as24.c as25.c as26.c as27.c as28.c as29.c

all:		as11 as11-2

install:	as11 as11-2
		cp as11 $(DESTDIR)/as11
		strip $(DESTDIR)/as11
		cp as11-2 $(LIBDIR)/as11-2
		strip $(LIBDIR)/as11-2

clean:
		rm -rf *.o core a.out as11 as11-2

lint:
		lint $(PASS1SRC)
		lint $(PASS2SRC)

as11:		$(PASS1)
		cc -o as11 $(CFLAGS) $(PASS1)

as11.o:		as11.c vars1.h
as12.o:		as12.c vars1.h
as13.o:		as13.c vars1.h stypes.h
as14.o:		as14.c vars1.h
as15.o:		as15.c vars1.h
as16.o:		as16.c vars1.h stypes.h
as17.o:		as17.c vars1.h
as18.o:		as18.c vars1.h
as19.o:		as19.c vars1.h stypes.h symbols.code

as11-2:		$(PASS2)
		cc -o as11-2 $(CFLAGS) $(PASS2)

as21.o:		as21.c vars2.h
as22.o:		as22.c vars2.h stypes.h
as23.o:		as23.c vars2.h
as24.o:		as24.c vars2.h
as25.o:		as25.c vars2.h
as26.o:		as26.c vars2.h stypes.h
as27.o:		as27.c vars2.h
as28.o:		as28.c vars2.h
as29.o:		as29.c vars2.h stypes.h symbols.code
