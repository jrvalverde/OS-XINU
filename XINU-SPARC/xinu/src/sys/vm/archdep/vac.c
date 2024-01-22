/* vac.c - routines for manipulating the Virtual Address Cache */
/*		- see also vacreg.s */

#include <enable.h>

/* space to save g1 thru g7 - used in vacreg.s */
int vac_sav_g1;
int vac_sav_g2;
int vac_sav_g3;
int vac_sav_g4;
int vac_sav_g5;
int vac_sav_g6;
int vac_sav_g7;

/*---------------------------------------------------------------------------
 * vac_Initialize - Initialize the VAC
 *---------------------------------------------------------------------------
 */
vac_Initialize()
{
    unsigned int	enable_reg;
    
    enable_reg = get_system_enable_reg();
    enable_reg &= ~ENA_CACHE;		/* turn the cache bit off */
    set_system_enable_reg(enable_reg);
    vac_clearTags();			/* clear the Tags */
}

/*---------------------------------------------------------------------------
 * vac_Enable - Turn the VAC on
 *---------------------------------------------------------------------------
 */
vac_Enable()
{
    unsigned int	enable_reg;
    
    enable_reg = get_system_enable_reg();
    if ( ! (enable_reg & ENA_CACHE) ) {	/* cache is disabled */
	vac_clearTags();
	enable_reg |= ENA_CACHE;	/* turn the cache bit on */
	set_system_enable_reg(enable_reg);
    }
}


/*---------------------------------------------------------------------------
 * vac_Disable - Turn the VAC off
 *---------------------------------------------------------------------------
 */
vac_Disable()
{
    unsigned int	enable_reg;
    
    enable_reg = get_system_enable_reg();
    if ( enable_reg & ENA_CACHE ) {		/* cache is enabled */
	vac_clearTags();
	enable_reg &= ~ENA_CACHE;	/* turn the cache bit off */
	set_system_enable_reg(enable_reg);
    }
}


/*---------------------------------------------------------------------------
 * vac_FlushCache - Clear the VAC
 *---------------------------------------------------------------------------
 */
vac_FlushCache()
{
    vac_clearTags();
}

