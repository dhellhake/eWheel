/* 
* System.cpp
*
* Created: 08.05.2018 18:22:30
* Author: dominik hellhake
*/


#include "System.h"

// static class member
volatile uint32_t System::SysTick_Overflow = 0;

// default constructor
System::System()
{
} //System

/**
 * Initialize the system
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
void System::Init()
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
	
	/* Configure SysTick-Counter */
	SysTick->CTRL = 0;					// Disable SysTick
	SysTick->LOAD = 0xFFFFFF;			// Set reload register for overflow interrupts
	NVIC_SetPriority(SysTick_IRQn, 3);	// Set interrupt priority to least urgency
	SysTick->VAL = 0;					// Reset the SysTick counter value
	SysTick->CTRL = 0x00000007;			// Enable SysTick, Enable SysTick Exceptions, Use CPU Clock
	NVIC_EnableIRQ(SysTick_IRQn);		// Enable SysTick Interrupt
	
	/* Configure Board-LED */
	PORT->Group[0].DIRSET.reg = PORT_PA28;
	PORT->Group[0].OUTCLR.reg = PORT_PA28;
	
	/* Configure External Interrupt */
	System::InitEIC();
}

void System::InitEIC()
{
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[EIC_GCLK_ID].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[EIC_GCLK_ID].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[EIC_GCLK_ID].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[EIC_GCLK_ID].reg |= GCLK_PCHCTRL_CHEN;
	
	// Reset EIC
	EIC->CTRLA.reg = EIC_CTRLA_SWRST;
	while (EIC->CTRLA.reg & EIC_CTRLA_SWRST);
	
	
	NVIC_SetPriority(EIC_IRQn, 0);;										
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)EIC_IRQn & 0x0000001f));	
	
	EIC->CONFIG[0].reg =	EIC_CONFIG_SENSE2_RISE |						//LSM6D-INT1
							EIC_CONFIG_SENSE7_BOTH;							//Hall Sensor
	EIC->CONFIG[1].reg =	EIC_CONFIG_SENSE4_BOTH	|
							EIC_CONFIG_SENSE5_BOTH;
	
	EIC->CTRLA.reg = EIC_CTRLA_ENABLE;
}
 
void System::SetPinPeripheralFunction(uint32_t pinmux)
{
	uint8_t port = (uint8_t)((pinmux >> 16)/32);
	 
	PORT->Group[port].PINCFG[((pinmux >> 16) - (port*32))].bit.PMUXEN = 1;
	 
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg &= ~(0xF << (4 * ((pinmux >> 16) & 0x01u)));
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg |= (uint8_t)((pinmux & 0x0000FFFF) << (4 * ((pinmux >> 16) & 0x01u)));
}

uint32_t System::GetGCLK_Hz(uint32_t gclk_id)
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

uint32_t System::GetElapsedMilis()
{
	uint64_t result = (System::SysTick_Overflow * 0xFFFFFF) + (0xFFFFFF - SysTick->VAL);
	return (uint32_t)(result / 48000);
}

void SysTick_Handler(void)
{
	System::SysTick_Overflow++;
}

// default destructor
System::~System()
{
} //~System
