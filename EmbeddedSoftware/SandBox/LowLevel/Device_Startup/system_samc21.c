#include "samc21.h"


/**
 * Initialize the system
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
void SystemInit()
 {
	 // Set flash wait states for operation at 48 MHz @ 5V
	 NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_RWS(1) | NVMCTRL_CTRLB_MANW;        //
	 while ((OSCCTRL->STATUS.reg & OSCCTRL_STATUS_OSC48MRDY) == 0 );       // Wait until ready
 
	 // Default setting for OSC48M
	 OSCCTRL->OSC48MCTRL.reg = OSCCTRL_OSC48MCTRL_ONDEMAND | OSCCTRL_OSC48MCTRL_ENABLE;
	 OSCCTRL->OSC48MDIV.reg = OSCCTRL_OSC48MDIV_DIV(1 - 1) | OSCCTRL_OSC48MCTRL_RUNSTDBY;   // 48MHz

	 REG_OSCCTRL_OSC48MSTUP = 0x07;              // ~21uS startup
	 while (OSCCTRL->OSC48MSYNCBUSY.reg);        // Wait until synced
	 while ((OSCCTRL->STATUS.reg & OSCCTRL_STATUS_OSC48MRDY) == 0 );        // Wait until ready

	 // Default setting for GEN0 (DIV => 0 & 1 are both 1)
	 GCLK->GENCTRL[0].reg = GCLK_GENCTRL_SRC_OSC48M | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_DIV(1);    // 48MHz
	 GCLK->GENCTRL[2].reg = GCLK_GENCTRL_SRC_OSC48M | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_DIV(6);    // 8MHz
 }