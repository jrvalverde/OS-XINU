#
#  Make the Xinu post mortum diagnostic program
#

DIR	=	/usr/Xinu
HDIR	=	$(DIR)/src/sys/h
DESTDIR =	$(DIR)/bin
CPP	=	cc -E
CFLAGS	=	-O
CFILES	=	pm.c setup.c trace.c ascdate.c
OFILES	=	pm.o setup.o trace.o ascdate.o
XHFILES	=	/usr/include/sys/exec.h	\
		/usr/include/a.out.h	\
		$(HDIR)/conf.h		\
		$(DIR)/include/coreX.h	\
		$(HDIR)/kernel.h	\
		$(HDIR)/q.h		\
		$(HDIR)/proc.h		\
		$(HDIR)/sem.h		\
		$(HDIR)/tty.h		\
		$(HDIR)/date.h

all:		pm

install:	pm
		chmod 755 pm
		cp pm $(DESTDIR)

clean:
		rm -f core pm *.o ,* x.h

x.h:		$(XHFILES)
		sed '/#define	NULL	/d;/#define	EOF/d; \
		     /#include \<sys\/exec.h\>/d;s/^#/+%+/' $(XHFILES) >,x
		sed 's/^+%+/#/;/^# /d;/^[ 	]*$$/d' < ,x > x.h
		rm ,x

pm:		x.h $(OFILES)
		$(CC) $(CFLAGS) -o pm $(OFILES)

depend:
		grep '^#include' $(CFILES) pm.h \
		| sed -e '/<.*>/d' -e 's/:[^"]*"\([^"]*\)".*/: \1/' \
		| sed 's/\.c/.o/' >makedep
		echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
		echo '$$r makedep' >>eddep
		echo 'w' >>eddep
		cp Makefile Makefile.bak
		ed - Makefile < eddep
		rm eddep makedep
		echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
		echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
		echo '# see make depend above' >> Makefile

# DO NOT DELETE THIS LINE
# DEPENDENCIES MUST END AT END OF FILE
pm.o: pm.h
setup.o: pm.h
trace.o: pm.h
ascdate.o: x.h
pm.h: x.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
