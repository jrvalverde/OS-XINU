#
#  Make cross-commands that run on the VAX
#

CFLAGS  = -O -s
DIR = ../..
DESTDIR = $(DIR)/bin
LIBDIR  = $(DIR)/lib

FILES=		cprint	

CFILES=		

OFILES=		

all:		$(FILES)

install:	$(FILES)
		chmod 755 $(FILES)
		cp $(FILES) $(DESTDIR)

clean:
		-rm -f *.o core	$(FILES)

cprint:		cprint.sh
		sed 's/^[ 	]*//;/^:/d' <cprint.sh >cprint


depend:
		grep '^#include' $(CFILES) \
		| sed -e '/<.*>/d' -e 's/:[^"]*"\([^"]*\)".*/: \1/' \
		| sed 's/\.c/.o/' >Makedep
		echo '/^# DO NOT DELETE	THIS LINE/+2,$$d' >eddep
		echo '$$r Makedep' >>eddep
		echo 'w' >>eddep
		cp Makefile ,Makefile
		ed - ,Makefile < eddep
		rm -rf  eddep Makedep Makefile.old
		echo '#	DEPENDENCIES MUST END AT END OF	FILE' >> ,Makefile
		echo '#	IF YOU PUT STUFF HERE IT WILL GO AWAY' >> ,Makefile
		echo '#	see make depend	above' >> ,Makefile
		cp Makefile Makefile.old
		mv ,Makefile Makefile

# DO NOT DELETE	THIS LINE
# DEPENDENCIES MUST END	AT END OF FILE
