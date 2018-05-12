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
		
	//Enable Interrupt on ECTINT1-3
	EIC->INTENSET.reg |= (1 << 1) | (1 << 2) | (1 << 3);
	
	/* Init Hall-State */
	MotorSensor::HallState = MotorSensor::MASKToState(PORT->Group[0].IN.reg >> 17);
}

void MotorSensor::Update()
{	
	MotorSensor::HallState = MotorSensor::MASKToState(PORT->Group[0].IN.reg >> 17);
}

// default destructor
MotorSensor::~MotorSensor()
{
} //~MotorSensor
