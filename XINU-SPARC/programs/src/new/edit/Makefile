#
# Makefile for Xinu Editor
#

PROGNAME =	edit

.SUFFIXES:.o .c .s

#
# Make variables
#
ROOTDIR	=	../../..
BIN	=	${ROOTDIR}/bin
LIB	=	${ROOTDIR}/lib
INC	=	${ROOTDIR}/include
SLIB	=	${LIB}/unixsyscall.a
CLIB	=	${LIB}/libxc.a
LD	=	/bin/ld -T 2020 -e _unixstartup
CC	=	/bin/cc
AS	=	/bin/as
CFLAGS	=	-g -I${INC}
#
# user's own C lib (usually compiled from their local unix source code)
#		 	Alternative C lib
#
ALTCLIB	=	${LIB}/stdiolibu.a ${LIB}/genlibu.a ${LIB}/stdiolibu.a 
ALTCFLAGS =	-g -DNORMAL_CLIB -I${INC}

.c.o:
		${CC} -c ${CFLAGS} $<
.s.o:
		${AS} $<

CFILES= deletechar.c deleteline.c movecharup.c movechardown.c \
       movecharleft.c movecharright.c movewordleft.c movewordright.c \
       enterinsertmode.c search.c putafter.c yank.c undolastdeletion.c \
       showhelpscreen.c showstatus.c gotobegofline.c gotoendofline.c \
       gototopoffile.c gotoendoffile.c readinputfile.c quit.c \
       meldlines.c gotoline.c \
       savefile.c scrolldn.c scrollup.c editor.c support.c \
	wyse50.c bogus.c
XOFILES= deletechar.o deleteline.o movecharup.o movechardown.o \
       movecharleft.o movecharright.o movewordleft.o movewordright.o \
       enterinsertmode.o search.o putafter.o yank.o undolastdeletion.o \
       showhelpscreen.o showstatus.o gotobegofline.o gotoendofline.o \
       gototopoffile.o gotoendoffile.o readinputfile.o quit.o \
       meldlines.o gotoline.o \
       savefile.o scrolldn.o scrollup.o editor.o support.o \
	wyse50.o bogus.o
HFILES = ed.h fn.h help.h

${PROGNAME}:	$(XOFILES)
		${LD} $(XOFILES) -lcurses -ltermlib ${CLIB} ${SLIB} -o ${PROGNAME}
alt${PROGNAME}:	$(XOFILES)
		${CC} -c ${CFLAGS} -DNORMAL_CLIB bogus.c 
		${LD} $(XOFILES) -lcurses -ltermlib ${ALTCLIB} ${SLIB} -o ${PROGNAME}

unixbinary:	$(XOFILES)
		/bin/cc ${XOFILES} -lcurses -ltermlib -o unix${PROGNAME}

install:	${PROGNAME}
		cp ${PROGNAME} ${BIN}/${PROGNAME}

clean:
		-rm -rf ${PROGNAME} *.o ,* .,* core 

depend:
		grep '^#include' *.c ../../h/*.h		\
		| sed	-e 's/</"..\/..\/h\//;s/>/"/'		\
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

#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
editor.o: ed.h chars.h fn.h
support.o: ed.h fn.h help.h
command.o: ed.h fn.h
deletechar.o: ed.h fn.h 
deleteline.o: ed.h fn.h 
movecharup.o: ed.h fn.h 
movechardown.o: ed.h fn.h 
movecharleft.o: ed.h fn.h 
movecharright.o: ed.h fn.h 
movewordleft.o: ed.h fn.h 
movewordright.o: ed.h fn.h 
enterinsertmode.o: ed.h fn.h 
search.o: ed.h fn.h 
putafter.o: ed.h fn.h 
yank.o: ed.h fn.h 
undolastdeletion.o: ed.h fn.h 
showhelpscreen.o: ed.h fn.h 
showstatus.o: ed.h fn.h 
gotobegofline.o: ed.h fn.h 
gotoendofline.o: ed.h fn.h 
gototopoffile.o: ed.h fn.h 
gotoendoffile.o: ed.h fn.h 
readinputfile.o: ed.h fn.h 
quit.o: ed.h fn.h 
meldlines.o: ed.h fn.h 
gotoline.o: ed.h fn.h 
savefile.o: ed.h fn.h 
scrolldn.o: ed.h fn.h 
scrollup.o: ed.h fn.h 
