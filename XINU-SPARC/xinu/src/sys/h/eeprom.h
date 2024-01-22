/* eeprom.h - eeprom addresses */


/*
 * The EEPROM is part of the Mostek MK48T02 clock chip. The EEPROM
 * is 2K, but the last 8 bytes are used as the clock, and the 32 bytes
 * before that emulate the ID prom. There is no
 * recovery time necessary after writes to the chip.
 */

/* EEPROM_ADDR taken from EEPROM in cpu.addrs.h */

#define EEPROM_ADDR	0xFFD04000	/* virtual address we map eeprom to */
#define	EEPROM_SIZE	0x7D8		/* size of eeprom in bytes */

/*
 * ID prom constants. They are included here because the ID prom is
 * emulated by stealing 20 bytes of the eeprom.
 */
#define	IDPROM_ADDR	(EEPROM_ADDR+EEPROM_SIZE) /* virtual addr of idprom */
#define	IDPROMSIZE	0x20			/* size of ID prom, in bytes */
/************************ end _sun4c_eeprom_h ***************************/
