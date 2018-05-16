/* 
* MotorDriver.cpp
*
* Created: 12.05.2018 13:25:12
* Author: dominik hellhake
*/


#include "MotorDriver.h"

volatile uint8_t MotorDriver::PhaseDuty = 0x00;

// default constructor
MotorDriver::MotorDriver()
{
} //MotorDriver

void MotorDriver::InitTC0()
{
	//Enable Clock for TC0
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.TC0_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[TC0_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[TC0_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[TC0_GCLK_ID].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[TC0_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	// Reset TC0
	TC0->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
	while (TC0->COUNT16.CTRLA.reg & TC_CTRLA_SWRST);
	
	TC0->COUNT8.CTRLA.reg = TC_CTRLA_MODE_COUNT8 | TC_CTRLA_PRESCALER_DIV8 | TC_CTRLA_PRESCSYNC_PRESC;		//8bit count mode; 48Mhz / Div8 = 6Mhz => ~23,5kHz
	TC0->COUNT8.WAVE.reg = TC_WAVE_WAVEGEN_NPWM;

	TC0->COUNT8.COUNT.reg = 0;
	TC0->COUNT8.CC[0].reg = 0;
	TC0->COUNT8.CC[1].reg = 0;
	
	TC0->COUNT8.CTRLA.reg |= TC_CTRLA_ENABLE;
	TC0->COUNT8.DBGCTRL.reg = TC_DBGCTRL_DBGRUN;
	
	//Configure PWM Output to V_High
	PORT->Group[0].DIRSET.reg = PORT_PA23;
	PORT->Group[0].OUTSET.reg = PORT_PA23;
	System::SetPinPeripheralFunction(PINMUX_PA23E_TC0_WO1);
	
	//Configure PWM Output to V_Low
	PORT->Group[0].DIRSET.reg = PORT_PA22;
	PORT->Group[0].OUTSET.reg = PORT_PA22;
	System::SetPinPeripheralFunction(PINMUX_PA22E_TC0_WO0);
}

void MotorDriver::InitTC1()
{
	//Enable Clock for TC1
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.TC1_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[TC1_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[TC1_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[TC1_GCLK_ID].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[TC1_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	// Reset TC1
	TC1->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
	while (TC1->COUNT16.CTRLA.reg & TC_CTRLA_SWRST);
		
	TC1->COUNT8.CTRLA.reg = TC_CTRLA_MODE_COUNT8 | TC_CTRLA_PRESCALER_DIV8 | TC_CTRLA_PRESCSYNC_PRESC;		//8bit count mode; 48Mhz / Div8 = 6Mhz => ~23,5kHz
	TC1->COUNT8.WAVE.reg = TC_WAVE_WAVEGEN_NPWM;

	TC1->COUNT8.COUNT.reg = 0;
	TC1->COUNT8.CC[0].reg = 0;
	TC1->COUNT8.CC[1].reg = 0;
		
	TC1->COUNT8.CTRLA.reg |= TC_CTRLA_ENABLE;
	TC1->COUNT8.DBGCTRL.reg = TC_DBGCTRL_DBGRUN;
	
	//Configure PWM Output to W_High
	PORT->Group[0].DIRSET.reg = PORT_PA25;
	PORT->Group[0].OUTSET.reg = PORT_PA25;
	System::SetPinPeripheralFunction(PINMUX_PA25E_TC1_WO1);
	
	//Configure PWM Output to W_Low
	PORT->Group[0].DIRSET.reg = PORT_PA24;
	PORT->Group[0].OUTSET.reg = PORT_PA24;
	System::SetPinPeripheralFunction(PINMUX_PA24E_TC1_WO0);
}

void MotorDriver::InitTC3()
{
	//Enable Clock for TC3
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.TC3_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[TC3_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[TC3_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[TC3_GCLK_ID].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[TC3_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	// Reset TC1
	TC3->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
	while (TC3->COUNT16.CTRLA.reg & TC_CTRLA_SWRST);
	
	TC3->COUNT8.CTRLA.reg = TC_CTRLA_MODE_COUNT8 | TC_CTRLA_PRESCALER_DIV8 | TC_CTRLA_PRESCSYNC_PRESC;		//8bit count mode; 48Mhz / Div8 = 6Mhz => ~23,5kHz
	TC3->COUNT8.WAVE.reg = TC_WAVE_WAVEGEN_NPWM;

	TC3->COUNT8.COUNT.reg = 0;
	TC3->COUNT8.CC[0].reg = 0;
	TC3->COUNT8.CC[1].reg = 0;
	
	TC3->COUNT8.CTRLA.reg |= TC_CTRLA_ENABLE;
	TC3->COUNT8.DBGCTRL.reg = TC_DBGCTRL_DBGRUN;
	
	//Configure PWM Output to W_High
	PORT->Group[0].DIRSET.reg = PORT_PA21;
	PORT->Group[0].OUTSET.reg = PORT_PA21;
	System::SetPinPeripheralFunction(PINMUX_PA21E_TC3_WO1);
	
	//Configure PWM Output to W_Low
	PORT->Group[0].DIRSET.reg = PORT_PA20;
	PORT->Group[0].OUTSET.reg = PORT_PA20;
	System::SetPinPeripheralFunction(PINMUX_PA20E_TC3_WO0);
}

// default destructor
MotorDriver::~MotorDriver()
{
} //~MotorDriver

