/*
 * TCC.c
 *
 * Created: 15.03.2019 15:45:13
 *  Author: Dominik Hellhake
 */
#include "TCC.h"

void InitTCC0()
{
	//Enable Clock for TC0
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.TCC0_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[TCC0_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[TCC0_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[TCC0_GCLK_ID].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[TCC0_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	
	TCC0->CTRLA.reg =	TCC_CTRLA_PRESCALER_DIV1 |			//No division of 48Mhz GCLK
						TCC_CTRLA_PRESCSYNC_GCLK;				//Count sync on GCLK
	
	TCC0->DBGCTRL.bit.DBGRUN = 1;							//Run on CPU-Halt during debug-session
	
	TCC0->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;
	
	TCC0->COUNT.reg	= 0x00;
	TCC0->PER.reg	= 2000;									//f_PWM = 48Mhz / 1 (2399 + 1)  => 20Khz
	
	TCC0->CC[0].reg	= 200;
	
	TCC0->WEXCTRL.reg = TCC_WEXCTRL_OTMX(0x2) |
						TCC_WEXCTRL_DTIEN0 |				//Enable Dead-Time-Insertion on WO0 (WO0 = High-Side; WO4 = Low-Side)
						TCC_WEXCTRL_DTIEN2 |				//Enable Dead-Time-Insertion on WO2 (WO2 = High-Side; WO6 = Low-Side)
						TCC_WEXCTRL_DTIEN3 |				//Enable Dead-Time-Insertion on WO3 (WO3 = High-Side; WO7 = Low-Side)
						TCC_WEXCTRL_DTLS(100) |				//Set Dead-Time-Insertion of 10x20,8ns (208ns) (min: 133ns)
						TCC_WEXCTRL_DTHS(150);				//Set Dead-Time-Insertion of 10x20,8ns (208ns) (min: 133ns)
	
	TCC0->CTRLA.bit.ENABLE = 1;
}
