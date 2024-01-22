#
#  Make man DEBUG ConcurrenC Manual Pages
#


MACROS	=	../tmac.an

FILES	=	$(MACROS)	\
		intro.d		\
		kprintf.d	\
		qdump.d		\
		stdump.d	\
		tdump.d

all:		$(FILES)

intro.d :	raw/intro.d.raw
		nroff ../tmac.an raw/intro.d.raw > intro.d
  
kprintf.d :	raw/kprintf.d.raw
		nroff ../tmac.an raw/kprintf.d.raw > kprintf.d
  
qdump.d :	raw/qdump.d.raw
		nroff ../tmac.an raw/qdump.d.raw > qdump.d
  
stdump.d :	raw/stdump.d.raw
		nroff ../tmac.an raw/stdump.d.raw > stdump.d
  
tdump.d :	raw/tdump.d.raw
		nroff ../tmac.an raw/tdump.d.raw > tdump.d
  
install:
		@echo no installation possible - use "'make print'"

clean:
		rm -rf *.d

print:		listings

listings:	$(FILES)
		cat *.d | sed '1,2d;s/^/	/' | lpr
