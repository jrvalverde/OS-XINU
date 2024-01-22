/* vcu.h */

/* vax console unit device constants */

#ifndef	VAX8600

#define	VCURXCSENBL	0x40		/* receive interrupt enable bit	*/
#define	VCURXCSDSBL	0x00		/* disable receive interrupts	*/
#define	VCUTXCSENBL	0x40		/* trans. interrupt enable bit	*/
#define	VCUTXCSDSBL	0x00		/* trans. interrupt disable mask*/

#else	VAX8600

#define	VCURXCSENBL	0x10040		/* enable receive interrupts	*/
#define	VCURXCSDSBL	0x10000		/* disable receive interrupts	*/
#define	VCUTXCSENBL	0x18040		/* enable transmit interrupts	*/
#define	VCUTXCSDSBL	0x18000		/* disable transmit interrupts	*/

#endif	VAX8600

#define	VCUREADY	0x80		/* device ready bit		*/
#define	VCUOCHMASK	0x7f		/* mask for output character	*/
#define	VCUICHMASK	0xff		/* mask for input character	*/
#define	VCURERMASK	0x8000		/* mask for recv error bit	*/
