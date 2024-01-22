#	{ConC-dir}/src/Makefile
#
# NOTE:
#
# There are two possible methods of doing host name look up.  They are
# to use the name server or /etc/hosts. HOSTLOOKUP defines which method
# is to be compiled into libc  :
#								     
#	defining HOSTLOOKUP to be "named", compiles the         
#	host lookup routines that use the BIND name server.
#
#	defining HOSTLOOKUP to be "hosttable", compiles
#	the host lookup routines that use /etc/hosts.
#
#HOSTLOOKUP=	hosttable
HOSTLOOKUP=	named

OBJS=	ConC_sys/*.o \
	compat-4.1/*.o \
	compat-sys5/*.o \
	gen/*.o \
	inet/*.o \
	net/*.o \
	net/${HOSTLOOKUP}/*.o \
	ns/*.o \
	stdio/*.o \
	sys_interface/*.o \
	vax/compat-4.1/*.o \
	vax/csu/*.o \
	vax/gen/*.o \
	vax/net/*.o \
	vax/stdio/*.o \
	vax/sys/*.o

ccclib.a: .ccclib.a
	cd ConC_sys          ; make ${MFLAGS}
	cd compat-4.1        ; make ${MFLAGS}
	cd compat-sys5       ; make ${MFLAGS}
	cd gen               ; make ${MFLAGS}
	cd inet              ; make ${MFLAGS}
	cd net               ; make ${MFLAGS}
	cd net/${HOSTLOOKUP} ; make ${MFLAGS}
	cd ns                ; make ${MFLAGS}
	cd stdio             ; make ${MFLAGS}
	cd sys_interface     ; make ${MFLAGS}
	cd vax/compat-4.1    ; make ${MFLAGS}
	cd vax/csu           ; make ${MFLAGS}
	cd vax/gen           ; make ${MFLAGS}
	cd vax/net           ; make ${MFLAGS}
	cd vax/stdio         ; make ${MFLAGS}
	cd vax/sys           ; make ${MFLAGS}
	@echo "***  Building src/ccclib.a  ..."
	@/bin/rm -f ./ccclib.a
	@(ar cru ./ccclib.a ${OBJS} 2>&1) > ,warnings
	@(ranlib ./ccclib.a         2>&1) > ,warnings
	@/bin/rm -f ,warnings

.ccclib.a:

lib_only:
	@echo "***  Building src/ccclib.a  ..."
	@/bin/rm -f ./ccclib.a
	@(ar cru ./ccclib.a ${OBJS} 2>&1) > ,warnings
	@(ranlib ./ccclib.a         2>&1) > ,warnings
	@/bin/rm -f ,warnings

install_ccclib.a:
	@echo "***  Building system ccclib.a  ..."
	@/bin/rm -f ../ccclib.a
	@cp ./ccclib.a ../ccclib.a
	@(ranlib ../ccclib.a 2>&1) > ,warnings
	@/bin/rm ,warnings

depend:
	cd ConC_sys          ; make ${MFLAGS} depend
	cd compat-4.1        ; make ${MFLAGS} depend
	cd compat-sys5       ; make ${MFLAGS} depend
	cd gen               ; make ${MFLAGS} depend
	cd inet              ; make ${MFLAGS} depend
	cd net               ; make ${MFLAGS} depend
	cd net/hosttable     ; make ${MFLAGS} depend
	cd net/named         ; make ${MFLAGS} depend
	cd ns                ; make ${MFLAGS} depend
	cd stdio             ; make ${MFLAGS} depend
	cd sys_interface     ; make ${MFLAGS} depend
	cd vax/compat-4.1    ; make ${MFLAGS} depend
	cd vax/csu           ; make ${MFLAGS} depend
	cd vax/gen           ; make ${MFLAGS} depend
	cd vax/net           ; make ${MFLAGS} depend
	cd vax/stdio         ; make ${MFLAGS} depend
	cd vax/sys           ; make ${MFLAGS} depend

clean:
	cd ConC_sys          ; make ${MFLAGS} clean
	cd compat-4.1        ; make ${MFLAGS} clean
	cd compat-sys5       ; make ${MFLAGS} clean
	cd gen               ; make ${MFLAGS} clean
	cd inet              ; make ${MFLAGS} clean
	cd net               ; make ${MFLAGS} clean
	cd net/hosttable     ; make ${MFLAGS} clean
	cd net/named         ; make ${MFLAGS} clean
	cd ns                ; make ${MFLAGS} clean
	cd stdio             ; make ${MFLAGS} clean
	cd sys_interface     ; make ${MFLAGS} clean
	cd vax/compat-4.1    ; make ${MFLAGS} clean
	cd vax/csu           ; make ${MFLAGS} clean
	cd vax/gen           ; make ${MFLAGS} clean
	cd vax/net           ; make ${MFLAGS} clean
	cd vax/stdio         ; make ${MFLAGS} clean
	cd vax/sys           ; make ${MFLAGS} clean
	/bin/rm -f  ./ccclib.a
	/bin/rm -f ../ccclib.a
