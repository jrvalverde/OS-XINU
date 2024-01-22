#LASERPRINTER = -Pcyplab
TROFF = troff
NROFF = nroff
TPREPROC	= soelim $*.ms |  pic | $(TROFF) -ms
NPREPROC	= soelim $*.ms |  pic | $(NROFF) -ms

.SUFFIXES:.print  .proof .ms .doc
.ms.print:;$(TPREPROC)  $(LASERPRINTER)
.ms.proof:;$(TPREPROC) -t$*.dit
.ms.doc:;$(NPREPROC) > $*.doc

doc: overview.doc

print: overview.print

dit: overview.proof


