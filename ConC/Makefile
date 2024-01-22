#	{ConC-dir}/Makefile
#

ccclib.a: .ccclib.a
	@echo "***  Making src/ccclib.a  ..."
	cd src		; make ${MFLAGS} ccclib.a
	@echo "***  Installing system ccclib.a  ..."
	cd src		; make ${MFLAGS} install_ccclib.a

.ccclib.a:

install: ccclib.a bin_dir example_dir man_dir
	@echo "  "
	@echo "  "
	@echo "Your ConcurrenC system is now installed."

bin_dir:
	@echo "***  Making bin directory  ..."
	@echo "C_PATH=`pwd`" > bin/ccc
	@echo "  " >> bin/ccc
	@echo 'cc -I$${C_PATH}/include -Dmain=_ccc__userproc \
				$$* $${C_PATH}/ccclib.a' >> bin/ccc
	@chmod 755 bin/ccc
	@echo "C_PATH=`pwd`" > bin/tccc
	@echo "  " >> bin/tccc
	@echo 'cc -I$${C_PATH}/include -Dmain=_ccc__userproc \
				$$* $${C_PATH}/src/ccclib.a' >> bin/tccc
	@chmod 755 bin/tccc

example_dir:
	@echo "***  Making example directory  ..."
	cd example	; make ${MFLAGS}

man_dir:
	@echo "***  Making man directory  ..."
	cd man	; make ${MFLAGS}

force:	clean depend install

depend:
	@echo "***  making depend for src  ..."
	cd src		; make ${MFLAGS} depend
	@echo "***  making depend for example  ..."
	cd example	; make ${MFLAGS} depend

clean:
	@echo "***  Cleaning src  ..."
	cd src		; make ${MFLAGS} clean
	@echo "***  Cleaning bin  ..."
	/bin/rm -f bin/ccc bin/tccc
	@echo "***  Cleaning example  ..."
	cd example	; make ${MFLAGS} clean
	@echo "***  Cleaning man  ..."
	cd man	; make ${MFLAGS} clean
