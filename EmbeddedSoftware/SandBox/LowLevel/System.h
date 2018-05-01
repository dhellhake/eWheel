/*
 * System.h
 *
 * Created: 30.04.2018 16:18:59
 *  Author: domin
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "sam.h"

static inline void pin_set_peripheral_function(uint32_t pinmux)
{
	uint8_t port = (uint8_t)((pinmux >> 16)/32);
	
	PORT->Group[port].PINCFG[((pinmux >> 16) - (port*32))].bit.PMUXEN = 1;
	
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg &= ~(0xF << (4 * ((pinmux >> 16) & 0x01u)));
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg |= (uint8_t)((pinmux & 0x0000FFFF) << (4 * ((pinmux >> 16) & 0x01u)));
}

static inline uint32_t get_gclk_hz(uint32_t gclk_id)
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

#endif /* SYSTEM_H_ */