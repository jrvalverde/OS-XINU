#
#  Make man1 ConcurrenC Manual Pages
#


MACROS	=	../tmac.an

FILES	=	$(MACROS)	\
		intro.1ccc	\
		ccc.1ccc	\
		ex1.1ccc ex2.1ccc ex3.1ccc ex4.1ccc ex5.1ccc

all:		$(FILES)

intro.1ccc:	raw/intro.1ccc.raw
		nroff ../tmac.an raw/intro.1ccc.raw > intro.1ccc

ccc.1ccc:	raw/ccc.1ccc.raw
		nroff ../tmac.an raw/ccc.1ccc.raw > ccc.1ccc

ex1.1ccc:	raw/ex1.1ccc.raw
		nroff ../tmac.an raw/ex1.1ccc.raw > ex1.1ccc

ex2.1ccc:	raw/ex2.1ccc.raw
		nroff ../tmac.an raw/ex2.1ccc.raw > ex2.1ccc

ex3.1ccc:	raw/ex3.1ccc.raw
		nroff ../tmac.an raw/ex3.1ccc.raw > ex3.1ccc

ex4.1ccc:	raw/ex4.1ccc.raw
		nroff ../tmac.an raw/ex4.1ccc.raw > ex4.1ccc

ex5.1ccc:	raw/ex5.1ccc.raw
		nroff ../tmac.an raw/ex5.1ccc.raw > ex5.1ccc

install:
		@echo no installation possible - use "'make print'"

clean:
		rm -rf *.1ccc

print:		listings

listings:	$(FILES)
		cat *.1ccc | sed '1,2d;s/^/	/' | lpr
