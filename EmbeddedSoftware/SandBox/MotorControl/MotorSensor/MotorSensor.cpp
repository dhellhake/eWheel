/* 
* MotorSensor.cpp
*
* Created: 12.05.2018 10:18:16
* Author: dominik hellhake
*/
#include "MotorSensor.h"

HALL_STATE MotorSensor::HallState = HALL_STATE::UNDEFINED_1;

// default constructor
MotorSensor::MotorSensor()
{
} //MotorSensor

void MotorSensor::InitEIC()
{		
	/* Pin-Config: U-Hall on EXTINT3 */
	PORT->Group[0].DIRCLR.reg = PORT_PA19;
	PORT->Group[0].PINCFG[PIN_PA19].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	System::SetPinPeripheralFunction(PINMUX_PA19A_EIC_EXTINT3);
	
	/* Pin-Config: V-Hall on EXTINT2 */
	PORT->Group[0].DIRCLR.reg = PORT_PA18;
	PORT->Group[0].PINCFG[PIN_PA18].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	System::SetPinPeripheralFunction(PINMUX_PA18A_EIC_EXTINT2);
	
	/* Pin-Config: W-Hall on EXTINT1 */
	PORT->Group[0].DIRCLR.reg = PORT_PA17;
	PORT->Group[0].PINCFG[PIN_PA17].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;	
	System::SetPinPeripheralFunction(PINMUX_PA17A_EIC_EXTINT1);
	
	
	/* Init Hall-State */
	MotorSensor::HallState = MotorSensor::MASKToState(PORT->Group[0].IN.reg >> 17);
	
			
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
	
	//Enable Interrupt for PIN
	EIC->INTENSET.reg = (1 << 1) | (1 << 2) | (1 << 3);					//Enable Interrupt on EXTINT1-3
	NVIC->ISER[0] = (uint32_t)(1 << ((uint32_t)EIC_IRQn & 0x0000001f));
	
	EIC->CONFIG[0].reg =	EIC_CONFIG_SENSE1_BOTH |						//Interrupt on Rise and Fall edge
							EIC_CONFIG_SENSE2_BOTH |						//Interrupt on Rise and Fall edge
							EIC_CONFIG_SENSE3_BOTH;							//Interrupt on Rise and Fall edge
	
	EIC->CTRLA.reg = EIC_CTRLA_ENABLE;
}

// default destructor
MotorSensor::~MotorSensor()
{
} //~MotorSensor

void EIC_Handler()
{	
	MotorSensor::HallState = MotorSensor::MASKToState(PORT->Group[0].IN.reg >> 17);
	EIC->INTFLAG.reg = (1 << 1) | (1 << 2) | (1 << 3);
}