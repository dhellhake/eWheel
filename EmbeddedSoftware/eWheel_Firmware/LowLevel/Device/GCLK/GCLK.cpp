/*
 * GCLK.c
 *
 * Created: 15.03.2019 15:22:38
 *  Author: Dominik Hellhake
 */ 
#include "GCLK.h"

void InitGCLK()
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
	GCLK->GENCTRL[1].reg = GCLK_GENCTRL_SRC_OSC48M | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_DIV(6);    // 8MHz
}


uint32_t GetGCLK_Hz(uint32_t gclk_id)
{
	//GCLK Frequency
	uint8_t gen_id = GCLK->PCHCTRL[gclk_id].bit.GEN;
	
	//Step 1:
	/* Get the frequency of the source connected to the GCLK generator */
	uint32_t gen_input_hz;
	
	switch (GCLK->GENCTRL[gen_id].bit.SRC)
	{
		case 6: //(GCLK_SOURCE_OSC48M)
		gen_input_hz = 48000000UL / (OSCCTRL->OSC48MDIV.bit.DIV + 1);
		break;
		default:
		gen_input_hz = 0;
	}
	
	uint8_t divsel = GCLK->GENCTRL[gen_id].bit.DIVSEL;
	uint32_t divider = GCLK->GENCTRL[gen_id].bit.DIV;
	
	if (!divsel && divider > 1) {
		gen_input_hz /= divider;
		} else if (divsel) {
		gen_input_hz >>= (divider+1);
	}
	
	return gen_input_hz;
}