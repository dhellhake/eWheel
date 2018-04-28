#include "samc21.h"


/**
 * Initialize the system
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
void SystemInit(void)
{	
	//Set INTFLAG to be cleared	
	SUPC->INTFLAG.reg = SUPC_INTFLAG_BODVDDRDY | SUPC_INTFLAG_BODVDDDET;
	
	//Set flash controller wait states.
	NVMCTRL->CTRLB.bit.RWS = 0;
	
	/* OSC48M */
	OSCCTRL->OSC48MCTRL.reg |= (1 << OSCCTRL_OSC48MCTRL_ONDEMAND_Pos)
								| (0 << OSCCTRL_OSC48MCTRL_RUNSTDBY_Pos);

	OSCCTRL->OSC48MDIV.reg = OSCCTRL_OSC48MDIV_DIV(1);
	while(OSCCTRL->OSC48MSYNCBUSY.reg);
		
		
	
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBAMASK.reg |= (1 << MCLK_APBAMASK_GCLK_Pos);	
	
	/* Software reset the module to ensure it is re-initialized correctly */
	GCLK->CTRLA.reg = GCLK_CTRLA_SWRST;
	while (GCLK->CTRLA.reg & GCLK_CTRLA_SWRST);
	
	
	//Set main CPU clock divider.
	MCLK->CPUDIV.reg = MCLK_CPUDIV_CPUDIV(0);
	
	
	//Initializes a Generic Clock Generator 0 to defaults.
	while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(1 << 0));
	
	GCLK->GENCTRL[0].reg = 
					6 << GCLK_GENCTRL_SRC_Pos |						//source_clock = GCLK_SOURCE_OSC48M;
					(GCLK->GENCTRL[0].reg & GCLK_GENCTRL_GENEN);
		
	while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(1 << 0));
	
	return;
}
