/* 
* MotorDriver.cpp
*
* Created: 12.05.2018 13:25:12
* Author: dominik hellhake
*/


#include "MotorDriver.h"

// default constructor
MotorDriver::MotorDriver()
{
} //MotorDriver

void MotorDriver::InitTC()
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
	
	
	
	
	TC1->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV2 | TC_CTRLA_PRESCSYNC_RESYNC;
	TC1->COUNT16.WAVE.reg = TC_WAVE_WAVEGEN_MPWM;				//Normal PWM

	TC1->COUNT16.COUNT.reg = 0;
	TC1->COUNT16.CC[0].reg = 1000;
	TC1->COUNT16.CC[1].reg = 500;
	
	
	TC1->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
	TC1->COUNT16.DBGCTRL.reg = TC_DBGCTRL_DBGRUN;
	
	//Configure PWM Output Pins
	PORT->Group[0].DIRSET.reg = PORT_PA25;
	PORT->Group[0].OUTSET.reg = PORT_PA25;
	System::SetPinPeripheralFunction(PINMUX_PA25E_TC1_WO1);
}

// default destructor
MotorDriver::~MotorDriver()
{
} //~MotorDriver
