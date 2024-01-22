/* mach.h - isUVAXI isUVAXII is1178X is11750 is11730 is8600		*/

/* vax machine constants						*/

#define UVAXIISID	0x08000000	/* SID reg. value for UVAX II	*/
#define UVAXISID	0x07000000	/* SID reg. value for UVAX I	*/
#define VAX78XSID	0x01000000
#define VAX750SID	0x02000000
#define VAX730SID	0x03000000
#define VAX8600SID	0x04000000

#define SYSTYPE		0x00ffffff	/* mask for system id		*/
#define UISYSTEM    	"MicroVAX I"	/* name printed at startup	*/
#define UIISYSTEM   	"MicroVAX II"	/* for either UVAX I or UVAX II	*/
#define SYSTEM78	"VAX 11/78X"
#define SYSTEM75	"VAX 11/750"
#define SYSTEM73	"VAX 11/730"
#define SYSTEM86	"VAX 8600"

/* Miscellaneous utility inline functions */

#ifndef	ASM
#define isUVAXII	(machinesid == UVAXIISID)
#define isUVAXI		(machinesid == UVAXISID)
#define is1178X		(machinesid == VAX78XSID)
#define is11750		(machinesid == VAX750SID)
#define is11730		(machinesid == VAX730SID)
#define is8600		(machinesid == VAX8600SID)
#endif	ASM

#ifndef ASM
extern	int	machinesid;
#endif	ASM
