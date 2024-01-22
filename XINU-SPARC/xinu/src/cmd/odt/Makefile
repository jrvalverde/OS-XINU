CFLAGS = -O -I./h
odt: odt.o libd.a
	cc -o odt odt.o libd.a

libd.a: lib/libd.a
	cd lib; make; cd ..

lib/libd.a:
	cd lib; make; cd ..

install: odt
	chmod 755 odt
	cp odt ../../../bin/odt
