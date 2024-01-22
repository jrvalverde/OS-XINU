#
#  Make man0 ConcurrenC Manual Pages
#


MACROS	=	../tmac.an

FILES	=	$(MACROS)	\
		intro.0ccc

all:		$(FILES)

intro.0ccc:	ms/intro.0ccc.ms
		nroff -ms ms/intro.0ccc.ms > intro.0ccc

install:
		@echo no installation possible - use "'make print'"

clean:
		rm -rf *.0ccc

print:		listings

listings:	$(FILES)
		cat *.0ccc | sed '1,2d;s/^/	/' | lpr
