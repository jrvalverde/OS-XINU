#
#  Make cross-commands that run on the VAX
#

CFLAGS	= -O -s
DIR	= /usr/Xinu
DESTDIR = $(DIR)/bin
LIBDIR  = $(DIR)/lib

FILES=		cprint

all:		$(FILES)

install:	$(FILES)
		chmod 755 $(FILES)
		cp $(FILES) $(DESTDIR)

clean:
		-rm -f *.o core	$(FILES)

cprint:		cprint.sh
		sed 's/^[ 	]*//;/^:/d' <cprint.sh >cprint

