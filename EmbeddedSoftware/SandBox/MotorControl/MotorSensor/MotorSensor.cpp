/* 
* MotorSensor.cpp
*
* Created: 12.05.2018 10:18:16
* Author: dominik hellhake
*/
#include "MotorSensor.h"

volatile HALL_STATE MotorSensor::HallState = HALL_STATE::UNDEFINED_1;

// default constructor
MotorSensor::MotorSensor()
{
} //MotorSensor

void MotorSensor::InitEIC()
{		
	/* Pin-Config: U-Hall on EXTINT13 */
	PORT->Group[0].DIRCLR.reg = PORT_PA25;
	PORT->Group[0].PINCFG[PIN_PA25].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	System::SetPinPeripheralFunction(PINMUX_PA25A_EIC_EXTINT13);
	
	/* Pin-Config: V-Hall on EXTINT12 */
	PORT->Group[0].DIRCLR.reg = PORT_PA24;
	PORT->Group[0].PINCFG[PIN_PA24].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	System::SetPinPeripheralFunction(PINMUX_PA24A_EIC_EXTINT12);
	
	/* Pin-Config: W-Hall on EXTINT7 */
	PORT->Group[0].DIRCLR.reg = PORT_PA23;
	PORT->Group[0].PINCFG[PIN_PA23].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;	
	System::SetPinPeripheralFunction(PINMUX_PA23A_EIC_EXTINT7);
		
	//Enable Interrupt on ECTINT 7, 12, 13
	EIC->INTENSET.reg |= (1 << 7) | (1 << 12) | (1 << 13);
}

void MotorSensor::HallState_Update()
{
	MotorSensor::HallState = MotorSensor::MASKToState((PORT->Group[0].IN.reg >> 23) & 0b111);
}


// default destructor
MotorSensor::~MotorSensor()
{
} //~MotorSensor
