#
#  Make All ConcurrenC Manual Pages
#

all:
		@echo making all manual pages in man[0123d]
		cd INTRO	; make ${MFLAGS}
		cd man0		; make ${MFLAGS}
		cd man1		; make ${MFLAGS}
		cd man2		; make ${MFLAGS}
		cd man3		; make ${MFLAGS}
		cd mand		; make ${MFLAGS}

install:
		@echo type "\`make print'" to make and print the entire manual

clean:
		cd INTRO	; make ${MFLAGS} clean
		cd man0		; make ${MFLAGS} clean
		cd man1		; make ${MFLAGS} clean
		cd man2		; make ${MFLAGS} clean
		cd man3		; make ${MFLAGS} clean
		cd mand		; make ${MFLAGS} clean

print:		manual

listings:	manual

manual:		all
		cat INTRO/INTRO man0/*.0* man1/*.1* man2/*.2* man3/*.3* \
			mand/*.d* | \ sed '1,2d;s/^/	/' | lpr
