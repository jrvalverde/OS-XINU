#
#  Make INTRO ConcurrenC Manual Pages
#


MACROS	=	../tmac.an

FILES	=	$(MACROS)	\
		INTRO

all:		$(FILES)

INTRO:		INTRO.ms
		nroff -ms INTRO.ms > INTRO

install:
		@echo no installation possible - use "'make print'"

clean:
		rm -rf INTRO

print:		listings

listings:	$(FILES)
		cat INTRO | sed '1,2d;s/^/	/' | lpr
