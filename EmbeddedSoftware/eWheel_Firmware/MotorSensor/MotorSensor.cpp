/* 
* MotorSensor.cpp
*
* Created: 12.05.2018 10:18:16
* Author: dominik hellhake
*/
#include "MotorSensor.h"

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT MotorSensor::Run()
{
	this->HallState = (HALL_STATE)((PORT->Group[0].IN.reg >> 23) & 0b111);
	
	return RUN_RESULT::SUCCESS;
}

void MotorSensor::Propagate()
{
	if (this->OLED != NULL)
	this->OLED->SetRow(this->StateToIndex(this->HallState), 0);
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
MotorSensor::MotorSensor()
{		
	/* Pin-Config: U-Hall on EXTINT13 */
	PORT->Group[0].DIRCLR.reg = PORT_PA25;
	PORT->Group[0].OUTSET.reg = PORT_PA25;
	PORT->Group[0].PINCFG[PIN_PA25].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	System::SetPinPeripheralFunction(PINMUX_PA25A_EIC_EXTINT13);
	
	/* Pin-Config: V-Hall on EXTINT12 */
	PORT->Group[0].DIRCLR.reg = PORT_PA24;
	PORT->Group[0].OUTSET.reg = PORT_PA24;
	PORT->Group[0].PINCFG[PIN_PA24].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	System::SetPinPeripheralFunction(PINMUX_PA24A_EIC_EXTINT12);
	
	/* Pin-Config: W-Hall on EXTINT7 */
	PORT->Group[0].DIRCLR.reg = PORT_PA23;
	PORT->Group[0].OUTSET.reg = PORT_PA23;
	PORT->Group[0].PINCFG[PIN_PA23].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;	
	System::SetPinPeripheralFunction(PINMUX_PA23A_EIC_EXTINT7);
		
	//Enable Interrupt on ECTINT 7, 12, 13
	EIC->INTENSET.reg |= (1 << 7) | (1 << 12) | (1 << 13);
} //MotorSensor