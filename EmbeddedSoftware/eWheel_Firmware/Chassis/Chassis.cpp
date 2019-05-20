/* 
* Chassis.cpp
*
* Created: 28.04.2019 20:30:49
* Author: Dominik Hellhake
*/
#include "Chassis.h"

Chassis Board;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT Chassis::Run(uint32_t timeStamp)
{
	this->Chassis_Pitch = Gyro.Pitch - this->Road_Pitch;
	this->Chassis_Roll = Gyro.Roll - this->Road_Pitch;	
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
Chassis::Chassis()
{
} //Chassis

void Chassis::SetLED(bool state)
{
	if (state)
		PORT->Group[0].OUTSET.reg = PORT_PA28;
	else
		PORT->Group[0].OUTCLR.reg = PORT_PA28;
}