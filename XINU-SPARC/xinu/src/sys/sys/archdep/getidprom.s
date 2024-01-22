! getidprom.s - read the idprom into the buffer addr
!
!----------------------------------------------------------------------
! getidprom(addr, size)
! register char *addr;
! register int size;
! {
!	while (size) {
!	    *addr++ = *idprom++; /* idprom is really in a different space */
!	    size--;
!	}
! }
!----------------------------------------------------------------------
!
! we are a leaf procedure, so don't bother with save/restore
! %o0=addr, %o1=size
! use other %o registers as locals
!

#include <asl.h>
#include <eeprom.h>

	.seg	"text"
	.proc 04
	PROCEDURE(getidprom)
	set	IDPROM_ADDR, %o4	! save address of ID_PROM in %o4
	clr	%o5			! set prom offset to 0
1:
	tst	%o1			! have we copied enough bytes yet
	be	2f
	nop
	ldub	[%o4 + %o5],%o3		! read in a byte from the ID_PROM
	sub	%o1,0x1,%o1		! dec size
	stb	%o3,[%o0]		! store byte to addr (parameter 0)
	add	%o5,0x1,%o5		! inc prom offset
	add	%o0,0x1,%o0		! inc addr offset
	b	1b
	nop
2:
	retl
	nop


