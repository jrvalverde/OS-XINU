#
#	Makefile for book version of stateless file server
#

DIR	=	/usr/Xinu
BINDIR	=	$(DIR)/bin

CFLAGS	=	-O -c

CFILES	=	Close.c Open.c Seek.c Uncache.c fgetargs.c fgetreq.c	\
		finit.c fmode.c fs.c fsaccess.c fsclose.c fsmkdir.c	\
		fsndrply.c fsopen.c fsread.c fsrename.c fsrmdir.c	\
		fstrans.c fsunlink.c fswrite.c openfile.c rmfile.c

OFILES	=	Close.o Open.o Seek.o Uncache.o fgetargs.o fgetreq.o	\
		finit.o fmode.o fs.o fsaccess.o fsclose.o fsmkdir.o	\
		fsndrply.o fsopen.o fsread.o fsrename.o fsrmdir.o	\
		fstrans.o fsunlink.o fswrite.o openfile.o rmfile.o

all:		fs fsd

install:	fs fsd
		chmod 755 fs fsd
		cp fs fsd $(BINDIR)

clean:
		rm -f *.o core fs fsd ,*

fs:		$(OFILES)
		cc -o fs $(OFILES)

fsd:		fsd.c
		cc -O -o fsd fsd.c

book:		.formatted

.formatted:	$(CFILES) fsd.c
		Doform
		touch .formatted

depend:
		grep '^#include' *.c *.h			\
		| sed	-e 's/</"\/usr\/include\//;s/>/"/'	\
			-e 's/:[^"]*"\([^"]*\)".*/: \1/'	\
		| sed	's/\.c/.o/' >makedep
		echo '/^# DO NOT DELETE	THIS LINE/+2,$$d' >eddep
		echo '$$r makedep' >>eddep
		echo 'w' >>eddep
		cp Makefile .,oldmake
		cp Makefile ,Makefile
		ed - ,Makefile < eddep
		/bin/rm eddep makedep
		echo '#	DEPENDENCIES MUST END AT END OF	FILE' >>,Makefile
		echo '#	IF YOU PUT STUFF HERE IT WILL GO AWAY'>>,Makefile
		echo '#	see make depend	above'>>,Makefile
		/bin/mv ,Makefile Makefile

# DO NOT DELETE	THIS LINE
# DEPENDENCIES MUST END	AT END OF FILE
Close.o: fs.h
Open.o: fs.h
Seek.o: fs.h
Uncache.o: fs.h
fgetargs.o: fs.h
fgetreq.o: fs.h
finit.o: fs.h
fmode.o: fs.h
fs.o: fs.h
fsaccess.o: fs.h
fsclose.o: fs.h
fsd.o: /usr/include/stdio.h
fsd.o: /usr/include/pwd.h
fsd.o: /usr/include/grp.h
fsmkdir.o: fs.h
fsndrply.o: fs.h
fsopen.o: fs.h
fsread.o: fs.h
fsrename.o: fs.h
fsrmdir.o: fs.h
fstrans.o: fs.h
fsunlink.o: fs.h
fswrite.o: fs.h
openfile.o: fs.h
rmfile.o: fs.h
fs.h: ../../sys/h/kernel.h
fs.h: ../../sys/h/file.h
fs.h: ../../sys/h/fserver.h
fs.h: /usr/include/sys/file.h
fs.h: /usr/include/sys/types.h
fs.h: /usr/include/sys/socket.h
fs.h: /usr/include/sys/stat.h
fs.h: /usr/include/net/if.h
fs.h: /usr/include/netinet/in.h
fs.h: /usr/include/stdio.h
fs.h: /usr/include/strings.h
fs.h: /usr/include/errno.h
fs.h: /usr/include/netdb.h
fs.h: /usr/include/pwd.h
fs.h: /usr/include/grp.h
fs.h: /usr/include/signal.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
