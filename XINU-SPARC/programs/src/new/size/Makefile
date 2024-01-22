# Makefile for GNU binary-file utilities
# select a set of CFLAGS and PROGS, below, depending on the system type
# Copyright (C) 1989, Free Software Foundation, Inc.
#
# This file is part of the GNU binutils.
# 
# The GNU binutils are free software; you can redistribute them and/or modify
# them under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 1, or (at your option)
# any later version.
# 
# The GNU binutils are distributed in the hope that they will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with the GNU binutils; see the file COPYING.  If not, write to
# the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.



PROGNAME =	size

.SUFFIXES:.o .c .s

#
# Make variables
#
ROOTDIR	=	../../..
BIN	=	$(ROOTDIR)/bin
LIB	=	$(ROOTDIR)/lib
INC	=	$(ROOTDIR)/include
SLIB	=	$(LIB)/unixsyscall.a
CLIB	=	$(LIB)/libxc.a
LD	=	/bin/ld -T 2020 -e _unixstartup
CC	=	/bin/cc
AS	=	/bin/as

# CDEFS gets defines, and gets passed to lint. CFLAGS gets flags, and doesn't
# get passed to lint.

# Do not include alloca.h on the sparcs - to do this, define _alloca_h
CDEFS=	-DXINU8 -D_alloca_h
CFLAGS=	-g -I$(INC) $(CDEFS)

OFILES=	size.o

INCS=

CFILES=	size.c

.c.o:
		$(CC) -c $(CFLAGS) $<
.s.o:
		$(AS) $<

$(PROGNAME):	$(OFILES)
		$(LD) $(OFILES) $(CLIB) $(SLIB) -o $(PROGNAME)

install:	$(PROGNAME)
		cp $(PROGNAME) $(BIN)/$(PROGNAME)

clean:
		rm -f $(PROGNAME) *.o ,* .,* core

lint: $(SRCS) $(INCS)
	lint -ahbz $(CDEFS) $(SRCS)

strip:
	strip $(PROGNAME)
